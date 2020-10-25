/* @(#)edc_ecc.c	1.31 13/04/28 Copyright 1998-2013 Heiko Eissfeldt, Joerg Schilling */
#include <schily/mconfig.h>
#ifndef lint
static	UConst char sccsid[] =
	"@(#)edc_ecc.c	1.31 13/04/28 Copyright 1998-2013 Heiko Eissfeldt, Joerg Schilling";
#endif

/*
 * Copyright 1998-2002,2008 by Heiko Eissfeldt
 * Copyright 2002-2013 by Joerg Schilling
 *
 * This file contains protected intellectual property.
 *
 * reed-solomon encoder / decoder for compact discs.
 *
 */
/*
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * See the file CDDL.Schily.txt in this distribution for details.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file CDDL.Schily.txt from this distribution.
 */

#include <schily/stdio.h>
#include <schily/align.h>
#include <schily/utypes.h>
#include <schily/stdlib.h>
#include <schily/string.h>
#include "ecc.h"

#ifndef	HAVE_MEMMOVE
#include <schily/libport.h>	/* Define missing prototypes */
/*#define	memmove(dst, src, size)		movebytes((src), (dst), (size))*/
#define	memmove(d, s, n)	bcopy((s), (d), (n))
#endif

/*
 * The functions do_decode_L1(), do_decode_L2(), decode_L1_P(), decode_L1_Q()
 * are currently static and limited to debug mode (#ifdef MAIN).
 * After they are ready, they will be made public without #ifdef MAIN.
 */
#ifdef	MAIN
/* these prototypes will become public when the function are implemented */
static int do_decode_L2	__PR((unsigned char in[(L2_RAW+L2_Q+L2_P)],
		unsigned char out[L2_RAW]));

static int do_decode_L1	__PR((unsigned char in[(L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR],
		unsigned char out[L1_RAW*FRAMES_PER_SECTOR],
		int delay1, int delay2, int delay3, int scramble));
#endif


/* ------------- tables generated by gen_encodes --------------*/

#include "scramble_table"

#define	DO4(a)	a; a; a; a;
#define	DO13(a)	a; a; a; a; a; a; a; a; a; a; a; a; a;

/*
 * Scrambles 2352 - 12 = 2340 bytes
 */
int scramble_L2 __PR((unsigned char *inout));

int
scramble_L2(inout)
	unsigned char *inout;
{
#ifndef	EDC_SCRAMBLE_NOSWAP
	unsigned int *f = (unsigned int *)inout;
#endif

	if (!xaligned(inout + 12, sizeof (UInt32_t)-1)) {

		Uchar		*r = inout + 12;
		const Uchar	*s = yellowbook_scrambler;
		register int	i;

		for (i = (L2_RAW + L2_Q + L2_P +16)/sizeof (unsigned char)/4; --i >= 0; ) {
			DO4(*r++ ^= *s++);
		}

	} else {
		UInt32_t	*r = (UInt32_t *) (inout + 12);
		const UInt32_t  *s = yellowbook_scrambler_uint32;
		register int	i;

		for (i = (L2_RAW + L2_Q + L2_P +16)/sizeof (UInt32_t)/13; --i >= 0; ) {
			DO13(*r++ ^= *s++);
		}
	}

#ifndef	EDC_SCRAMBLE_NOSWAP

	/* generate F1 frames */
	for (i = 2352/sizeof (unsigned int); i; i--) {
		*f++ = ((*f & 0xff00ff00UL) >> 8) | ((*f & 0x00ff00ffUL) << 8);
	}
#endif

	return (0);
}

#include "l2sq_table"

int encode_L2_Q	__PR((unsigned char inout[4 + L2_RAW + 4 + 8 + L2_P + L2_Q]));

int
encode_L2_Q(inout)
	unsigned char inout[4 + L2_RAW + 4 + 8 + L2_P + L2_Q];
{
	unsigned char *dps;
	unsigned char *dp;
	unsigned char *Q;
	register int i;
	int j;

	Q = inout + 4 + L2_RAW + 4 + 8 + L2_P;

	dps = inout;
	for (j = 0; j < 26; j++) {
		register unsigned short a;
		register unsigned short b;
		a = b = 0;

		dp = dps;
		for (i = 0; i < 43; i++) {

			/* LSB */
			a ^= L2sq[i][*dp++];

			/* MSB */
			b ^= L2sq[i][*dp];

			dp += 2*44-1;
			if (dp >= &inout[(4 + L2_RAW + 4 + 8 + L2_P)]) {
				dp -= (4 + L2_RAW + 4 + 8 + L2_P);
			}
		}
		Q[0]	  = a >> 8;
		Q[26*2]	  = a;
		Q[1]	  = b >> 8;
		Q[26*2+1] = b;

		Q += 2;
		dps += 2*43;
	}
	return (0);
}

int encode_L2_P	__PR((unsigned char inout[4 + L2_RAW + 4 + 8 + L2_P]));

int
encode_L2_P(inout)
	unsigned char inout[4 + L2_RAW + 4 + 8 + L2_P];
{
	unsigned char *dp;
	unsigned char *P;
	register int i;
	int j;

	P = inout + 4 + L2_RAW + 4 + 8;

	for (j = 0; j < 43; j++) {
		register unsigned short a;
		register unsigned short b;

		a = b = 0;
		dp = inout;
		for (i = 19; i < 43; i++) {

			/* LSB */
			a ^= L2sq[i][*dp++];

			/* MSB */
			b ^= L2sq[i][*dp];

			dp += 2*43 -1;
		}
		P[0]	  = a >> 8;
		P[43*2]	  = a;
		P[1]	  = b >> 8;
		P[43*2+1] = b;

		P += 2;
		inout += 2;
	}
	return (0);
}

static unsigned char bin2bcd	__PR((unsigned p));

static unsigned char
bin2bcd(p)
	unsigned p;
{
	return ((p/10)<<4)|(p%10);
}

static int build_address	__PR((unsigned char inout[], int sectortype, unsigned address));

static int
build_address(inout, sectortype, address)
	unsigned char inout[];
	int sectortype;
	unsigned address;
{
	inout[12] = bin2bcd(address / (60*75));
	inout[13] = bin2bcd((address / 75) % 60);
	inout[14] = bin2bcd(address % 75);
	if (sectortype == MODE_0)
		inout[15] = 0;
	else if (sectortype == MODE_1)
		inout[15] = 1;
	else if (sectortype == MODE_2)
		inout[15] = 2;
	else if (sectortype == MODE_2_FORM_1)
		inout[15] = 2;
	else if (sectortype == MODE_2_FORM_2)
		inout[15] = 2;
	else
		return (-1);
	return (0);
}

#include "crctable.out"

/*
 * Called with 2064, 2056 or 2332 byte difference - all dividable by 4.
 */
unsigned int build_edc __PR((unsigned char inout[], int from, int upto));

unsigned int
build_edc(inout, from, upto)
	unsigned char inout[];
	int from;
	int upto;
{
	unsigned char *p = inout+from;
	unsigned int result = 0;

	upto -= from-1;
	upto /= 4;
	while (--upto >= 0) {
		result = EDC_crctable[(result ^ *p++) & 0xffL] ^ (result >> 8);
		result = EDC_crctable[(result ^ *p++) & 0xffL] ^ (result >> 8);
		result = EDC_crctable[(result ^ *p++) & 0xffL] ^ (result >> 8);
		result = EDC_crctable[(result ^ *p++) & 0xffL] ^ (result >> 8);
	}
	return (result);
}

/* Layer 2 Product code en/decoder */
int do_encode_L2 __PR((unsigned char inout[(12 + 4 + L2_RAW+4+8+L2_Q+L2_P)], int sectortype, unsigned address));

int
do_encode_L2(inout, sectortype, address)
	unsigned char inout[(12 + 4 + L2_RAW+4+8+L2_Q+L2_P)];
	int sectortype;
	unsigned address;
{
	unsigned int result;

/*	SYNCPATTERN	"\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff" */
#define	SYNCPATTERN	"\000\377\377\377\377\377\377\377\377\377\377"

	/* supply initial sync pattern */
	memcpy(inout, SYNCPATTERN, sizeof (SYNCPATTERN));

	if (sectortype == MODE_0) {
		memset(inout + sizeof (SYNCPATTERN), 0, 4 + L2_RAW + 12 + L2_P + L2_Q);
		build_address(inout, sectortype, address);
		return (0);
	}

	switch (sectortype) {

	case MODE_1:
		build_address(inout, sectortype, address);
		result = build_edc(inout, 0, 16+2048-1);
		inout[2064+0] = result >> 0L;
		inout[2064+1] = result >> 8L;
		inout[2064+2] = result >> 16L;
		inout[2064+3] = result >> 24L;
		memset(inout+2064+4, 0, 8);
		encode_L2_P(inout+12);
		encode_L2_Q(inout+12);
		break;
	case MODE_2:
		build_address(inout, sectortype, address);
		break;
	case MODE_2_FORM_1:
		result = build_edc(inout, 16, 16+8+2048-1);
		inout[2072+0] = result >> 0L;
		inout[2072+1] = result >> 8L;
		inout[2072+2] = result >> 16L;
		inout[2072+3] = result >> 24L;

		/* clear header for P/Q parity calculation */
		inout[12] = 0;
		inout[12+1] = 0;
		inout[12+2] = 0;
		inout[12+3] = 0;
		encode_L2_P(inout+12);
		encode_L2_Q(inout+12);
		build_address(inout, sectortype, address);
		break;
	case MODE_2_FORM_2:
		build_address(inout, sectortype, address);
		result = build_edc(inout, 16, 16+8+2324-1);
		inout[2348+0] = result >> 0L;
		inout[2348+1] = result >> 8L;
		inout[2348+2] = result >> 16L;
		inout[2348+3] = result >> 24L;
		break;
	default:
		return (-1);
	}

	return (0);
}


/*--------------------------------------------------------------------------*/
#include "encoder_tables"

static int encode_L1_Q	__PR((unsigned char inout[L1_RAW + L1_Q]));

static int
encode_L1_Q(inout)
	unsigned char inout[L1_RAW + L1_Q];
{
	unsigned char *Q;
	int	i;

	memmove(inout+L1_RAW/2+L1_Q, inout+L1_RAW/2, L1_RAW/2);
	Q = inout + L1_RAW/2;

	memset(Q, 0, L1_Q);
	for (i = 0; i < L1_RAW; i++) {
		unsigned char data;

		if (i == L1_RAW/2) {
			inout += L1_Q;
		}
		data = inout[i];
		if (data != 0) {
			unsigned char base = rs_l12_log[data];

			Q[0] ^= rs_l12_alog[(base+AQ[0][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
			Q[1] ^= rs_l12_alog[(base+AQ[1][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
			Q[2] ^= rs_l12_alog[(base+AQ[2][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
			Q[3] ^= rs_l12_alog[(base+AQ[3][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
		}
	}
	return (0);
}

static int encode_L1_P	__PR((unsigned char inout[L1_RAW + L1_Q + L1_P]));

static int
encode_L1_P(inout)
	unsigned char inout[L1_RAW + L1_Q + L1_P];
{
	unsigned char *P;
	int	i;

	P = inout + L1_RAW + L1_Q;

	memset(P, 0, L1_P);
	for (i = 0; i < L1_RAW + L1_Q + L1_P; i++) {
		unsigned char data;

		data = inout[i];
		if (data != 0) {
			unsigned char base = rs_l12_log[data];

			P[0] ^= rs_l12_alog[(base+AP[0][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
			P[1] ^= rs_l12_alog[(base+AP[1][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
			P[2] ^= rs_l12_alog[(base+AP[2][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
			P[3] ^= rs_l12_alog[(base+AP[3][i]) % (unsigned)((1 << RS_L12_BITS)-1)];
		}
	}
	return (0);
}

#ifdef	MAIN	/* Make public (non-static) if completely implemented */

static int decode_L1_Q	__PR((unsigned char inout[L1_RAW + L1_Q]));

static int
decode_L1_Q(inout)
	unsigned char inout[L1_RAW + L1_Q];
{
	return (0);
}

static int decode_L1_P	__PR((unsigned char in[L1_RAW + L1_Q + L1_P]));

static int
decode_L1_P(in)
	unsigned char in[L1_RAW + L1_Q + L1_P];
{
	return (0);
}
#endif	/* MAIN */

int decode_L2_Q __PR((unsigned char inout[4 + L2_RAW + 12 + L2_Q]));

int
decode_L2_Q(inout)
	unsigned char inout[4 + L2_RAW + 12 + L2_Q];
{
	return (0);
}

int decode_L2_P __PR((unsigned char inout[4 + L2_RAW + 12 + L2_Q + L2_P]));

int
decode_L2_P(inout)
	unsigned char inout[4 + L2_RAW + 12 + L2_Q + L2_P];
{
	return (0);
}

static int encode_LSUB_Q	__PR((unsigned char inout[LSUB_RAW + LSUB_Q]));

static int
encode_LSUB_Q(inout)
	unsigned char inout[LSUB_RAW + LSUB_Q];
{
	unsigned char *Q;
	int i;

	memmove(inout+LSUB_QRAW+LSUB_Q, inout+LSUB_QRAW, LSUB_RAW-LSUB_QRAW);
	Q = inout + LSUB_QRAW;

	memset(Q, 0, LSUB_Q);

	for (i = 0; i < LSUB_QRAW; i++) {
		unsigned char data;

		data = inout[i] & 0x3f;
		if (data != 0) {
			unsigned char base = rs_sub_rw_log[data];

			Q[0] ^= rs_sub_rw_alog[(base+SQ[0][i]) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			Q[1] ^= rs_sub_rw_alog[(base+SQ[1][i]) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
		}
	}
	return (0);
}


static int encode_LSUB_P	__PR((unsigned char inout[LSUB_RAW + LSUB_Q + LSUB_P]));

static int
encode_LSUB_P(inout)
	unsigned char inout[LSUB_RAW + LSUB_Q + LSUB_P];
{
	unsigned char *P;
	int i;

	P = inout + LSUB_RAW + LSUB_Q;

	memset(P, 0, LSUB_P);
	for (i = 0; i < LSUB_RAW + LSUB_Q; i++) {
		unsigned char data;

		data = inout[i] & 0x3f;
		if (data != 0) {
			unsigned char base = rs_sub_rw_log[data];

			P[0] ^= rs_sub_rw_alog[(base+SP[0][i]) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			P[1] ^= rs_sub_rw_alog[(base+SP[1][i]) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			P[2] ^= rs_sub_rw_alog[(base+SP[2][i]) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			P[3] ^= rs_sub_rw_alog[(base+SP[3][i]) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
		}
	}
	return (0);
}

int decode_LSUB_Q __PR((unsigned char inout[LSUB_QRAW + LSUB_Q]));

int
decode_LSUB_Q(inout)
	unsigned char inout[LSUB_QRAW + LSUB_Q];
{
	unsigned char Q[LSUB_Q];
	int i;

	memset(Q, 0, LSUB_Q);
	for (i = LSUB_QRAW + LSUB_Q -1; i >= 0; i--) {
		unsigned char data;

		data = inout[LSUB_QRAW + LSUB_Q -1 -i] & 0x3f;
		if (data != 0) {
			unsigned char base = rs_sub_rw_log[data];

			Q[0] ^= rs_sub_rw_alog[(base+0*i) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			Q[1] ^= rs_sub_rw_alog[(base+1*i) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
		}
	}
	return (Q[0] != 0 || Q[1] != 0);
}

int decode_LSUB_P __PR((unsigned char inout[LSUB_RAW + LSUB_Q + LSUB_P]));

int
decode_LSUB_P(inout)
	unsigned char inout[LSUB_RAW + LSUB_Q + LSUB_P];
{
	unsigned char P[LSUB_P];
	int i;

	memset(P, 0, LSUB_P);
	for (i = LSUB_RAW + LSUB_Q + LSUB_P-1; i >= 0; i--) {
		unsigned char data;

		data = inout[LSUB_RAW + LSUB_Q + LSUB_P -1 -i] & 0x3f;
		if (data != 0) {
			unsigned char base = rs_sub_rw_log[data];

			P[0] ^= rs_sub_rw_alog[(base+0*i) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			P[1] ^= rs_sub_rw_alog[(base+1*i) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			P[2] ^= rs_sub_rw_alog[(base+2*i) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
			P[3] ^= rs_sub_rw_alog[(base+3*i) % (unsigned)((1 << RS_SUB_RW_BITS)-1)];
		}
	}
	return (P[0] != 0 || P[1] != 0 || P[2] != 0 || P[3] != 0);
}

/* Layer 1 CIRC en/decoder */
#define	MAX_L1_DEL1	2
static unsigned char l1_delay_line1[MAX_L1_DEL1][L1_RAW];
#define	MAX_L1_DEL2	108
static unsigned char l1_delay_line2[MAX_L1_DEL2][L1_RAW+L1_Q];
#define	MAX_L1_DEL3	1
static unsigned char l1_delay_line3[MAX_L1_DEL3][L1_RAW+L1_Q+L1_P];
static unsigned l1_del_index;

int do_encode_L1  __PR((unsigned char in[L1_RAW*FRAMES_PER_SECTOR],
		unsigned char out[(L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR],
		int delay1, int delay2, int delay3, int permute));

int
do_encode_L1(in, out, delay1, delay2, delay3, permute)
	unsigned char in[L1_RAW*FRAMES_PER_SECTOR];
	unsigned char out[(L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR];
	int delay1;
	int delay2;
	int delay3;
	int permute;
{
	int i;

	for (i = 0; i < FRAMES_PER_SECTOR; i++) {
		int j;
		unsigned char t;

		if (in != out)
			memcpy(out, in, L1_RAW);

		if (delay1) {
			/* shift through delay line 1 */
			for (j = 0; j < L1_RAW; j++) {
				if (((j/4) % MAX_L1_DEL1) == 0) {
					t = l1_delay_line1[l1_del_index % (MAX_L1_DEL1)][j];
					l1_delay_line1[l1_del_index % (MAX_L1_DEL1)][j] = out[j];
					out[j] = t;
				}
			}
		}

		if (permute) {
			/* permute */
			t = out[2]; out[2] = out[8]; out[8] = out[10]; out[10] = out[18];
			out[18] = out[6]; out [6] = t;
			t = out[3]; out[3] = out[9]; out[9] = out[11]; out[11] = out[19];
			out[19] = out[7]; out [7] = t;
			t = out[4]; out[4] = out[16]; out[16] = out[20]; out[20] = out[14];
			out[14] = out[12]; out [12] = t;
			t = out[5]; out[5] = out[17]; out[17] = out[21]; out[21] = out[15];
			out[15] = out[13]; out [13] = t;
		}

		/* build Q parity */
		encode_L1_Q(out);

		if (delay2) {
			/* shift through delay line 2 */
			for (j = 0; j < L1_RAW+L1_Q; j++) {
				if (j != 0) {
					t = l1_delay_line2[(l1_del_index) % MAX_L1_DEL2][j];
					l1_delay_line2[(l1_del_index + j*4) % MAX_L1_DEL2][j] = out[j];
					out[j] = t;
				}
			}
		}

		/* build P parity */
		encode_L1_P(out);

		if (delay3) {
			/* shift through delay line 3 */
			for (j = 0; j < L1_RAW+L1_Q+L1_P; j++) {
				if (((j) & MAX_L1_DEL3) == 0) {
					t = l1_delay_line3[0][j];
					l1_delay_line3[0][j] = out[j];
					out[j] = t;
				}
			}
		}

		/* invert Q and P parity */
		for (j = 0; j < L1_Q; j++)
			out[j+12] = ~out[j+12];
		for (j = 0; j < L1_P; j++)
			out[j+28] = ~out[j+28];

		l1_del_index++;
		out += L1_RAW+L1_Q+L1_P;
		in += L1_RAW;
	}
	return (0);
}

#ifdef	MAIN	/* Make public (non-static) if completely implemented */

static /* XXX should be non static XXX*/

int
do_decode_L1(in, out, delay1, delay2, delay3, permute)
	unsigned char in[(L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR];
	unsigned char out[L1_RAW*FRAMES_PER_SECTOR];
	int delay1;
	int delay2;
	int delay3;
	int permute;
{
	int i;

	for (i = 0; i < FRAMES_PER_SECTOR; i++) {
		int j;
		unsigned char t;

		if (delay3) {
			/* shift through delay line 3 */
			for (j = 0; j < L1_RAW+L1_Q+L1_P; j++) {
				if (((j) & MAX_L1_DEL3) != 0) {
					t = l1_delay_line3[0][j];
					l1_delay_line3[0][j] = in[j];
					in[j] = t;
				}
			}
		}

		/* invert Q and P parity */
		for (j = 0; j < L1_Q; j++)
			in[j+12] = ~in[j+12];
		for (j = 0; j < L1_P; j++)
			in[j+28] = ~in[j+28];

		/* build P parity */
		decode_L1_P(in);

		if (delay2) {
			/* shift through delay line 2 */
			for (j = 0; j < L1_RAW+L1_Q; j++) {
				if (j != L1_RAW+L1_Q-1) {
					t = l1_delay_line2[(l1_del_index) % MAX_L1_DEL2][j];
					l1_delay_line2[(l1_del_index + (MAX_L1_DEL2 - j*4)) % MAX_L1_DEL2][j] = in[j];
					in[j] = t;
				}
			}
		}

		/* build Q parity */
		decode_L1_Q(in);

		if (permute) {
			/* permute */
			t = in[2]; in[2] = in[6]; in[6] = in[18]; in[18] = in[10];
			in[10] = in[8]; in [8] = t;
			t = in[3]; in[3] = in[7]; in[7] = in[19]; in[19] = in[11];
			in[11] = in[9]; in [9] = t;
			t = in[4]; in[4] = in[12]; in[12] = in[14]; in[14] = in[20];
			in[20] = in[16]; in [16] = t;
			t = in[5]; in[5] = in[13]; in[13] = in[15]; in[15] = in[21];
			in[21] = in[17]; in [17] = t;
		}

		if (delay1) {
			/* shift through delay line 1 */
			for (j = 0; j < L1_RAW; j++) {
				if (((j/4) % MAX_L1_DEL1) != 0) {
					t = l1_delay_line1[l1_del_index % (MAX_L1_DEL1)][j];
					l1_delay_line1[l1_del_index % (MAX_L1_DEL1)][j] = in[j];
					in[j] = t;
				}
			}
		}

		if (in != out)
			memcpy(out, in, (L1_RAW));

		l1_del_index++;
		in += L1_RAW+L1_Q+L1_P;
		out += L1_RAW;
	}
	return (0);
}

static int
do_decode_L2(in, out)
	unsigned char in[(L2_RAW+L2_Q+L2_P)];
	unsigned char out[L2_RAW];
{
	return (0);
}
#endif	/* MAIN */


#define	MAX_SUB_DEL	8
static unsigned char sub_delay_line[MAX_SUB_DEL][LSUB_RAW+LSUB_Q+LSUB_P];
static unsigned sub_del_index;

/* R-W Subchannel en/decoder */

int do_encode_sub __PR((unsigned char in[LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME],
		unsigned char out[(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME],
		int delay1, int permute));

int
do_encode_sub(in, out, delay1, permute)
	unsigned char in[LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME];
	unsigned char out[(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME];
	int delay1;
	int permute;
{
	int i;

	if (in == out)
		return (-1);

	for (i = 0; i < PACKETS_PER_SUBCHANNELFRAME; i++) {
		int j;
		unsigned char t;

		memcpy(out, in, (LSUB_RAW));

		/* build Q parity */
		encode_LSUB_Q(out);

		/* build P parity */
		encode_LSUB_P(out);

		if (permute) {
			/* permute */
			t = out[1]; out[1] = out[18]; out[18] = t;
			t = out[2]; out[2] = out[ 5]; out[ 5] = t;
			t = out[3]; out[3] = out[23]; out[23] = t;
		}

		if (delay1) {
			/* shift through delay_line */
			for (j = 0; j < LSUB_RAW+LSUB_Q+LSUB_P; j++) {
				if ((j % MAX_SUB_DEL) != 0) {
					t = sub_delay_line[(sub_del_index) % MAX_SUB_DEL][j];
					sub_delay_line[(sub_del_index + j) % MAX_SUB_DEL][j] = out[j];
					out[j] = t;
				}
			}
		}
		sub_del_index++;
		out += LSUB_RAW+LSUB_Q+LSUB_P;
		in += LSUB_RAW;
	}
	return (0);
}

int
do_decode_sub __PR((
	unsigned char in[(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME],
	unsigned char out[LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME],
	int delay1, int permute));

int
do_decode_sub(in, out, delay1, permute)
	unsigned char in[(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME];
	unsigned char out[LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME];
	int delay1;
	int permute;
{
	int i;

	if (in == out)
		return (-1);

	for (i = 0; i < PACKETS_PER_SUBCHANNELFRAME; i++) {
		int j;
		unsigned char t;

		if (delay1) {
			/* shift through delay_line */
			for (j = 0; j < LSUB_RAW+LSUB_Q+LSUB_P; j++) {
				if ((j % MAX_SUB_DEL) != MAX_SUB_DEL-1) {
					t = sub_delay_line[(sub_del_index) % MAX_SUB_DEL][j];
					sub_delay_line[(sub_del_index + (MAX_SUB_DEL - j)) % MAX_SUB_DEL][j] = in[j];
					in[j] = t;
				}
			}
		}

		if (permute) {
			/* permute */
			t = in[1]; in[1] = in[18]; in[18] = t;
			t = in[2]; in[2] = in[ 5]; in[ 5] = t;
			t = in[3]; in[3] = in[23]; in[23] = t;
		}

		/* build P parity */
		decode_LSUB_P(in);

		/* build Q parity */
		decode_LSUB_Q(in);

		memcpy(out, in, LSUB_QRAW);
		memcpy(out+LSUB_QRAW, in+LSUB_QRAW+LSUB_Q, LSUB_RAW-LSUB_QRAW);

		sub_del_index++;
		in += LSUB_RAW+LSUB_Q+LSUB_P;
		out += LSUB_RAW;
	}
	return (0);
}

static int sectortype = MODE_0;

int get_sector_type __PR((void));

int
get_sector_type()
{
	return (sectortype);
}

int set_sector_type __PR((int st));

int
set_sector_type(st)
	int st;
{
	switch (st) {

	case MODE_0:
	case MODE_1:
	case MODE_2:
	case MODE_2_FORM_1:
	case MODE_2_FORM_2:
		sectortype = st;
		return (0);
	default:
		return (-1);
	}
}

/* ------------- --------------*/
#ifdef MAIN

#define	DO_L1	1
#define	DO_L2	2
#define	DO_SUB	4

static const unsigned sect_size[8][2] = {
/* nothing */
{0, 0},
/* Layer 1 decode/encode */
{ (L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR, L1_RAW*FRAMES_PER_SECTOR},
/* Layer 2 decode/encode */
{ 16+L2_RAW+12+L2_Q+L2_P, L2_RAW},
/* Layer 1 and 2 decode/encode */
{ (L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR, L1_RAW*FRAMES_PER_SECTOR},
/* Subchannel decode/encode */
{ (LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME,
	LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME},
/* Layer 1 and subchannel decode/encode */
{ (L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR +
	(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME,
	LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME +
	    L1_RAW*FRAMES_PER_SECTOR},
/* Layer 2 and subchannel decode/encode */
{ L2_RAW+L2_Q+L2_P +
	(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME,
	LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME +
	    L2_RAW},
/* Layer 1, 2 and subchannel decode/encode */
{ (L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR +
	(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME,
	LSUB_RAW*PACKETS_PER_SUBCHANNELFRAME +
	L1_RAW*FRAMES_PER_SECTOR},
};

int main	__PR((int argc, char **argv));

int
main(argc, argv)
	int	argc;
	char	**argv;
{
	int encode = 1;
	int mask = DO_L2;
	FILE *infp;
	FILE *outfp;
	unsigned address = 0;
	unsigned char *l1_inbuf;
	unsigned char *l1_outbuf;
	unsigned char *l2_inbuf;
	unsigned char *l2_outbuf;
	unsigned char *sub_inbuf;
	unsigned char *sub_outbuf;
	unsigned char *last_outbuf;
	unsigned char inbuf[(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME +
			(L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR];
	unsigned char outbuf[(LSUB_RAW+LSUB_Q+LSUB_P)*PACKETS_PER_SUBCHANNELFRAME +
			(L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR];
	unsigned load_offset;

	l1_inbuf = l2_inbuf = sub_inbuf = inbuf;
	l1_outbuf = l2_outbuf = sub_outbuf = last_outbuf = outbuf;

	infp = fopen("sectors_in", "rb");
	outfp = fopen("sectors_out", "wb");

	sectortype = MODE_1;
	address = 0 + 75*2;

	switch (sectortype) {

	case MODE_1:
	case MODE_2:
		load_offset = 16;
		break;
	case MODE_2_FORM_1:
	case MODE_2_FORM_2:
		load_offset = 24;
		break;
	default:
		load_offset = 0;
	}
	while (1) {

		if (1 != fread(inbuf+load_offset,
				sect_size[mask][encode], 1, infp)) {
			perror("");
			break;
		}
		if (encode == 1) {
			if (mask & DO_L2) {
				switch (sectortype) {

				case MODE_0:
					break;
				case MODE_1:
					break;
				case MODE_2:
					if (1 !=
					    fread(inbuf+load_offset+
						sect_size[mask][encode],
					    2336 - sect_size[mask][encode],
						1, infp)) { perror(""); break; }
					break;
				case MODE_2_FORM_1:
					break;
				case MODE_2_FORM_2:
					if (1 !=
					    fread(inbuf+load_offset+
						sect_size[mask][encode],
					    2324 - sect_size[mask][encode],
						1, infp)) { perror(""); break; }
					break;
				default:
					if (1 !=
					    fread(inbuf+load_offset+
						sect_size[mask][encode],
					    2448 - sect_size[mask][encode],
						1, infp)) { perror(""); break; }
					memset(inbuf, 0, 16);
					/*memset(inbuf+16+2048,0,12+272);*/
					break;
				}
				do_encode_L2(l2_inbuf, MODE_1, address);
				if (0) scramble_L2(l2_inbuf);
				last_outbuf = l1_inbuf = l2_inbuf;
				l1_outbuf = l2_inbuf;
				sub_inbuf = l2_inbuf + L2_RAW;
				sub_outbuf = l2_outbuf + 12 + 4+ L2_RAW+4+ 8+ L2_Q+L2_P;
			}
			if (mask & DO_L1) {
				do_encode_L1(l1_inbuf, l1_outbuf, 1, 1, 1, 1);
				last_outbuf = l1_outbuf;
				sub_inbuf = l1_inbuf + L1_RAW*FRAMES_PER_SECTOR;
				sub_outbuf = l1_outbuf + (L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR;
			}
			if (mask & DO_SUB) {
				do_encode_sub(sub_inbuf, sub_outbuf, 0, 0);
			}
		} else {
			if (mask & DO_L1) {
				do_decode_L1(l1_inbuf, l1_outbuf, 1, 1, 1, 1);
				last_outbuf = l2_inbuf = l1_outbuf;
				l2_outbuf = l1_inbuf;
				sub_inbuf = l1_inbuf + (L1_RAW+L1_Q+L1_P)*FRAMES_PER_SECTOR;
				sub_outbuf = l1_outbuf + L1_RAW*FRAMES_PER_SECTOR;
			}
			if (mask & DO_L2) {
				do_decode_L2(l2_inbuf, l2_outbuf);
				last_outbuf = l2_outbuf;
				sub_inbuf = l2_inbuf + L2_RAW+L2_Q+L2_P;
				sub_outbuf = l2_outbuf + L2_RAW;
			}
			if (mask & DO_SUB) {
				do_decode_sub(sub_inbuf, sub_outbuf, 1, 1);
			}
		}
		if (1 != fwrite(last_outbuf, sect_size[mask][1 - encode], 1, outfp)) {
			perror("");
			break;
		}
		address++;
	}
#if 0
	/* flush the data from the delay lines with zeroed sectors, if necessary */
#endif
	fclose(infp);
	fclose(outfp);

	return (0);
}
#endif
