��    0      �  C         (     )     /     K  !   d     �     �     �  &   �     �       �     �   �  �   �  	   I     S     _     k     t     �     �     �     �     �  "   �  �   �     �     �     �  �   �  �   Z	     �	  1   �	     
     %
     .
  �   N
  B   )  1   l     �  5   �     �     �       �    )   �            �       �  !   �     �  5         B  (   c     �  /   �     �     �  �   �  �   �  �   �  	   {     �     �     �     �     �     �     �     �     �  0   �  �        �     �     �  �     �   �       0   �     �     �  '   �  �   �  C   �  9        M  D   j     �     �     �  �  �  1   a!     �!     �!                       	   &                                             /   )      +          
      $          #         "                   !         0      .                   (   %                       *           ,   '            -    %B %Y %s \- manual page for %s %s %s: can't create %s (%s) %s: can't get `%s' info from %s%s %s: can't open `%s' (%s) %s: can't unlink %s (%s) %s: error writing to %s (%s) %s: no valid information found in `%s' AUTHOR AVAILABILITY Additional material may be included in the generated output with the
.B \-\-include
and
.B \-\-opt\-include
options.  The format is simple:

    [section]
    text

    /pattern/
    text
 Any
.B [NAME]
or
.B [SYNOPSIS]
sections appearing in the include file will replace what would have
automatically been produced (although you can still override the
former with
.B --name
if required).
 Blocks of verbatim *roff text are inserted into the output either at
the start of the given
.BI [ section ]
(case insensitive), or after a paragraph matching
.BI / pattern /\fR.
 COPYRIGHT DESCRIPTION ENVIRONMENT EXAMPLES Environment Examples FILES Files Games INCLUDE FILES Include file for help2man man page Lines before the first section or pattern which begin with `\-' are
processed as options.  Anything else is silently ignored and may be
used for comments, RCS keywords and the like.
 NAME OPTIONS Options Other sections are prepended to the automatically produced output for
the standard sections given above, or included at
.I other
(above) in the order they were encountered in the include file.
 Patterns use the Perl regular expression syntax and may be followed by
the
.IR i ,
.I s
or
.I m
modifiers (see
.BR perlre (1)).
 REPORTING BUGS Report +(?:[\w-]+ +)?bugs|Email +bug +reports +to SEE ALSO SYNOPSIS System Administration Utilities The full documentation for
.B %s
is maintained as a Texinfo manual.  If the
.B info
and
.B %s
programs are properly installed at your site, the command
.IP
.B info %s
.PP
should give you access to the complete manual.
 The latest version of this distribution is available on-line from: The section output order (for those included) is: This +is +free +software Try `--no-discard-stderr' if option outputs to stderr Usage User Commands Written +by `%s' generates a man page out of `--help' and `--version' output.

Usage: %s [OPTION]... EXECUTABLE

 -n, --name=STRING       description for the NAME paragraph
 -s, --section=SECTION   section number for manual page (1, 6, 8)
 -m, --manual=TEXT       name of manual (User Commands, ...)
 -S, --source=TEXT       source of program (FSF, Debian, ...)
 -L, --locale=STRING     select locale (default "C")
 -i, --include=FILE      include material from `FILE'
 -I, --opt-include=FILE  include material from `FILE' if it exists
 -o, --output=FILE       send output to `FILE'
 -p, --info-page=TEXT    name of Texinfo manual
 -N, --no-info           suppress pointer to Texinfo manual
 -l, --libtool           exclude the `lt-' from the program name
     --help              print this help, then exit
     --version           print version number, then exit

EXECUTABLE should accept `--help' and `--version' options and produce output on
stdout although alternatives may be specified using:

 -h, --help-option=STRING     help option string
 -v, --version-option=STRING  version option string
 --version-string=STRING      version string
 --no-discard-stderr          include stderr when parsing option output

Report bugs to <bug-help2man@gnu.org>.
 help2man \- generate a simple manual page or other Project-Id-Version: help2man-1.40.7
Report-Msgid-Bugs-To: Brendan O'Dea <bug-help2man@gnu.org>
POT-Creation-Date: 2013-06-06 08:26+1000
PO-Revision-Date: 2012-04-05 22:17+0200
Last-Translator: Sergio Zanchetta <primes2h@ubuntu.com>
Language-Team: Italian <tp@lists.linux.it>
Language: it
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=2; plural=(n != 1);
 %B %Y %s \- pagina di manuale per %s %s %s: impossibile creare %s (%s) %s: impossibile ottenere informazioni su "%s" da %s%s %s: impossibile aprire "%s" (%s) %s: impossibile fare l'unlink di %s (%s) %s: errore scrivendo su %s (%s) %s: nessuna informazione valida trovata in "%s" AUTORE DISPONIBILITÀ Nell'output generato possono essere inclusi materiali aggiuntivi con le opzioni
.B \-\-include
e
.BR \-\-opt\-include .
Il formato è semplice:

    [sezione]
    testo

    /modello/
    testo
 Qualsiasi sezione
.B [NOME]
o
.B [SINTASSI]
che compare nel file di inclusione andrà a sostituire quelle che sarebbero
prodotte automaticamente (anche se è sempre possibile annullare le
prime con
.B --name
se necessario).
 Blocchi di testo letterale *roff sono inseriti all'interno dell'output
all'inizio della
.BI [ sezione ]
fornita (ignorando la differenza tra maiuscole e minuscole)
oppure dopo un paragrafo che corrisponde al
.BI / modello /\fR.
 COPYRIGHT DESCRIZIONE VARIABILI D'AMBIENTE ESEMPI Variabili d'ambiente Esempi FILE File Giochi FILE DI INCLUSIONE File di inclusione per la pagina man di help2man Le righe precedenti la prima sezione o i modelli che iniziano con "\-" sono
elaborati come opzioni.  Qualsiasi altra cosa è ignorata in silenzio e può essere
usata per commenti, parole chiave RCS e simili.
 NOME OPZIONI Opzioni Altre sezioni sono inserite prima di quelle automaticamente prodotte in output
per le sezioni standard fornite precedentemente, oppure sono incluse in
.I altre
(come sopra) nell'ordine riscontrato nel file di inclusione.
 I modelli usano la sintassi delle espressioni regolari Perl e possono essere seguiti dai
modificatori
.IR i ,
.I s
oppure
.I m
(consultare
.BR perlre (1)).
 SEGNALAZIONE BUG Segnalare +i +bug|Inviare +le +segnalazioni +bug VEDERE ANCHE SINTASSI Utilità di amministrazione del sistema L'intera documentazione di
.B %s
è mantenuta come un manuale Texinfo.  Se i programmi
.B info
e
.B %s
sono installati correttamente, il comando
.IP
.B info %s
.PP
dovrebbe dare accesso al manuale completo.
 L'ultima versione di questa distribuzione è disponibile online su: L'ordine di output delle sezioni (per quelle incluse) è: Questo +è +software +libero Usare "--no-discard-stderr" se l'opzione produce un output su stderr Uso Comandi utente Scritto +da "%s" genera una pagina man dall'output di "--help" e "--version".

Uso: %s [OPZIONE]... ESEGUIBILE

 -n, --name=STRINGA      Descrizione per il paragrafo NOME
 -s, --section=SEZIONE   Numero di sezione per la pagina di manuale (1, 6, 8)
 -m, --manual=TESTO      Nome del manuale (Comandi utente, ...)
 -S, --source=TESTO      Provenienza del programma (FSF, Debian, ...)
 -L, --locale=STRINGA    Seleziona la localizzazione (predefinita "C")
 -i, --include=FILE      Include il materiale proveniente dal "FILE"
 -I, --opt-include=FILE  Include il materiale proveniente dal "FILE" se esiste
 -o, --output=FILE       Invia l'output su "FILE"
 -p, --info-page=TESTO   Nome del manuale Texinfo
 -N, --no-info           Elimina il riferimento al manuale Texinfo
 -l, --libtool           Esclude "lt-" dal nome del programma
     --help              Stampa questo aiuto ed esce
     --version           Stampa il numero di versione ed esce

L'ESEGUIBILE dovrebbe accettare le opzioni "--help" e "--version" e produrre output su
stdout sebbene le alternative possano essere specificate usando:

 -h, --help-option=STRINGA     Stringa con opzione alternativa per l'aiuto
 -v, --version-option=STRINGA  Stringa con opzione alternativa per la versione
 --version-string=STRINGA      Stringa della versione
 --no-discard-stderr           Include lo stderr nell'analisi dell'output dell'opzione

Segnalare i bug a <bug-help2man@gnu.org>.
 help2man \- genera una semplice pagina di manuale o altre 