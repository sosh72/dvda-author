��    0      �  C         (     )     /     K  !   d     �     �     �  &   �     �       �     �   �  �   �  	   I     S     _     k     t     �     �     �     �     �  "   �  �   �     �     �     �  �   �  �   Z	     �	  1   �	     
     %
     .
  �   N
  B   )  1   l     �  5   �     �     �       �    )   �                   &  4   -  5   b  A   �  3   �  >     9   M  S   �  
   �     �     �  R    -  o     �     �     �     �     �     �               $     -  V   K  6  �  
   �     �     �  P  �  �   O  '   +  n   S  !   �     �  A   �  Q  /  e   �  S   �  .   ;      j      �   #   �      !  �  6!  <   )     Z)     a)                       	   &                                             /   )      +          
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
PO-Revision-Date: 2012-05-02 10:03+0200
Last-Translator: Мирослав Николић <miroslavnikolic@rocketmail.com>
Language-Team: Serbian <gnu@prevod.org>
Language: sr
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
 %B %Y. %s \— страница упутства за %s %s %s: не могу да направим „%s“ (%s) %s: не могу да добавим „%s“ инфо са %s%s %s: не могу да отворим „%s“ (%s) %s: не могу да поништим везу „%s“ (%s) %s: грешка приликом писања у %s (%s) %s: нисам пронашао исправне информације у „%s“ АУТОР ДОСТУПНОСТ Додатни материјал може бити укључен у створени резултат употребом
.B \-\-include
и
.B \-\-opt\-include
опцијом.  Облик је једноставан:

    [одељак]
    текст

    /шаблон/
    текст
 Сваки
.B [НАЗИВ]
или
.B [УВОД]
одељак који се појави у датотеци укључивања ће заменити оно што је
самостално произведено (ипак можете још увек да прескочите
модел са
.B --name
ако је затражено).
 Блокови дословног *рофф текста су уметнути у резултату било на
почетку датог
.BI [ одељка ]
(неосетљив на величину слова), или након поклапања пасуса са
.BI / шаблоном /\fR.
 АУТОРСКА ПРАВА ОПИС ОКРУЖЕЊЕ ПРИМЕРИ Окружење Примери ДАТОТЕКЕ Датотеке Игре УКЉУЧИ ДАТОТЕКЕ Укључи датотеку за страницу упутства хелп2мана Редови пре првог одељка или шаблона који почињу са „\-“ се
обрађују као опције.  Све остало се немо занемарује и може бити
коришћено за напомене, РЦС кључне речи и слично.
 НАЗИВ ОПЦИЈЕ Опције Остали одељци су прикачени самостално произведеном резултату за
стандардне одељке дате изнад, или укључени у
.I осталим
(изнад) по редоследу како су проналажени у датотеци укључивања.
 Шаблони користе синтаксу регуларног израза Перла и за њима могу да
стоје
.IR i ,
.I s
или
.I m
измењивачи (погледајте
.BR perlre (1)).
 ПРИЈАВЉИВАЊЕ ГРЕШАКА Пријавите +(?:[\w-]+ +)?грешке|Пошаљите +извештаје +о +грешкама +на ПОГЛЕДАЈТЕ ТАКОЂЕ УВОД Помагала за администрацију система Потпуна документација за
.B %s
је одржавана као Тексинфо упутство.  Ако су
.B инфо
и
.B %s
исправно инсталирани на вашем сајту, наредба
.IP
.B инфо %s
.PP
треба да вам да приступ потпуном упутству.
 Најновије издање ове расподеле је доступно на мрежи са: Поредак резултата одељка (за оне укључене) је: Ово +је +слободан +софтвер Покушајте „--no-discard-stderr“ ако опција шаље резултат у стандардну грешку Употреба Корисничке наредбе Написали +су „%s“ ствара странице упутства из „--help“ и „--version“ излаза.

Употреба: %s [ОПЦИЈА]... ИЗВРШНИ

 -n, --name=НИСКА            опис пасуса НАЗИВ
 -s, --section=ОДЕЉАК        број одељка за страницу упутства (1, 6, 8)
 -m, --manual=ТЕКСТ          назив упутства (Корисничке наредбе, ...)
 -S, --source=ТЕКСТ          извор програма (3ЦЦ, Дебијан, ...)
 -L, --locale=НИСКА          бира локалитет (основно је "C")
 -i, --include=ДАТОТЕКА      укључује материјал из „ДАТОТЕКЕ“
 -I, --opt-include=ДАТОТЕКА  укључује материјал из ДАТОТЕКЕ ако постоји
 -o, --output=ДАТОТЕКА       шаље излаз у ДАТОТЕКУ
 -p, --info-page=ТЕКСТ       назив Тексинфо упутства
 -N, --no-info               потискује указивач на Тексинфо упутства
 -l, --libtool               искључује „lt-“ из назива програма
     --help                  исписује ову помоћ, затим излази
     --version               исписује број издања, затим излази

ИЗВРШНИ треба да прихвати „--help“ и „--version“ опције и да произведе излаз
на стандардном излазу иако алтернативе могу бити наведене користећи:

 -h, --help-option=НИСКА     ниска опције помоћи
 -v, --version-option=НИСКА  ниска опције издања
 --version-string=НИСКА      ниска издања
 --no-discard-stderr         укључује стандардну грешку при обради излаза опције

Грешке пријавите на <bug-help2man@gnu.org>.
 help2man \— ствара страницу упутства или остало 