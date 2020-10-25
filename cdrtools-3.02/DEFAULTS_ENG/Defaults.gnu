#ident "@(#)Defaults.gnu	1.6 13/11/04 "
###########################################################################
#
# global definitions for GNU (hurd) Systems
#
###########################################################################
#
# Compiler stuff
#
###########################################################################
#DEFCCOM=	cc
DEFCCOM=	gcc

###########################################################################
#
# Link mode for libraries that are part of the makefile system:
# If DEFLINKMODE is set to "static", no dynamic linking will be used
# If DEFLINKMODE is set to "dynamic", dynamic linking will be used
#
###########################################################################
DEFLINKMODE=	static

###########################################################################
#
# If the next line is commented out, compilation is done with max warn level
# If the next line is uncommented, compilation is done with minimal warnings
#
###########################################################################
#CWARNOPTS=

DEFINCDIRS=	$(SRCROOT)/include
DEFOSINCDIRS=	/usr/src/linux/include
LDPATH=		-L/opt/schily/lib
#RUNPATH=	-R$(INS_BASE)/lib -R/opt/schily/lib -R$(OLIBSDIR)
RUNPATH=	-R$(INS_BASE)/lib -R/opt/schily/lib

###########################################################################
#
# Installation config stuff
#
###########################################################################
#INS_BASE=	/opt/schily
#INS_KBASE=	/
#INS_RBASE=	/
INS_BASE=	/tmp/schily
INS_KBASE=	/tmp/schily/root
INS_RBASE=	/tmp/schily/root
#
DEFUMASK=	002
#
DEFINSMODEF=	444
DEFINSMODEX=	755
DEFINSUSR=	root
DEFINSGRP=	bin
