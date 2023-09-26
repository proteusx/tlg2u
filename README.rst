====
Repository moved to Codeberg
====
https://codeberg.org/proteusx/tlg2u

=====
TLG2U
=====
A command line utility to decode beta encoded classical text and print to
the standard output.

DESCRIPTION
-----------

tlg2gu converts beta encoded classical texts, from the
`Thesaurus Linguae Graecae <http://stephanus.tlg.uci.edu/tlgauthors/cd.authors.php>`_ (TLG) and the Packard Humanities Institute (PHI5/7) `Latin
<https://web.archive.org/web/20170623105104/http://latin.packhum.org/canon>`_,
`epigraphical <http://epigraphy.packhum.org/>`_ and `papyrological
<http://library.duke.edu/rubenstein/scriptorium/papyrus/texts/greek/introduction.html>`_
corpora in CD-ROM, to utf-8 encoded text suitable for further processing by
word processor, Latex, etc., and prints the results to the standard output.
It can convert whole files when giving a filename or single texts by
specifying the corpus, the author id and the work id.

`Beta Code <http://stephanus.tlg.uci.edu/encoding.php>`_ is an character encoding
for classical texts where text and formatting information onto combinations
of ascii characters whereas indexing information is stored as binary data
interleaved with the ascii data.

To use tlg2u you will need to have the CD-ROMs containing the text corpora.
As far as I know these are no longer sold but they are easily found via bit
torrent.

Optionally tlg2u can emit text with Latex markups (option -p).  You need to
add appropriate preamble and postamble before passing this output through
`XeLaTex <https://tug.org/xetex/>`_ to produce a pdf document.

Although tlg2u can be easily used as a standalone utility, it is
meant to be used in conjunction other commands or with a GUI utility
through which the user can select authors and works from a list.


COMPILING
---------

tlg2u is written in C and to compile it you need `GNU Make
<https://www.gnu.org/software/make/>`_ and the `GCC <https://gcc.gnu.org/>`_
compiler.  If compiling for Micro$oft Windows you will need the `MinGW GCC
<http://www.mingw.org/>`_ compiler instead. `Groff
<https://www.gnu.org/software/groff/>`_ is also needed to build the
documentation.

* Download the tlg2u source from GitHub:

.. code::

  git clone https://github.com:proteusx/tlg2u
  cd tlg2u/src

* Edit the makefile depending on whether you are compiling for Linux
  or Windows, 32 bit or 64 bit.  The makefile defaults to 64bit Linux.

* Issue the command ``make`` to compile the source.  This will build the
  documentation and  the ELF executable ``tlg2u`` (or the MZ executable
  ``tlg2u.exe`` for Windows) which can be run in situ or copied somewhere
  in your existing search path.


EXAMPLES OF USAGE
-----------------

The following examples assume that you have access to the CD-ROMs through the directory ``./cdroms/``

``tlg2u -a 0012 -b 001 -c tlg -d ./cdroms/tlge > Iliad.utf``

    Convert Homer's Iliad to utf-8 encoded text (the default). Homer
    is found in the TLG CDROM with
    author id=0012 and the Iliad has book id=001. The path of the
    input file is ./cdroms/tlge/tlg0012.txt. The results are sent to
    the file Iliad.utf which can be viewed with Open/LibreOffice Writer or
    any other unicode capable editor/word processor.


``tlg2u -a 0448 -b 001 -L -C -c lat -d ./cdroms/phi5 > DeBello.txt``

    Convert Caesar's De Bello Galico to utf-8. Append all citation
    numbers on the left margin. The input file is
    ./cdroms/phi5/lat0448.txt.


``tlg2u -a 0016 -b 001 -P -l -c tlg -d ./cdroms/phi5 > herod_hist.txt``

    Convert Herodotus' Historiae, append section numbers to the left
    margin and line numbers to the right margin. The input file is
    ./cdroms/tlge/tlg0448.txt.


``tlg2u -L doccan2.txt > canon.txt``

    Translate the TLG canon to unicode and write to file canon.txt
    (i.e. print the author catalogue).  You need to do this first to know
    the author and book ids.
    The -L option is set
    because the canon contains mainly Latin text. The few titles in
    Greek are indicated in the source file by language change marks and
    so will be printed correctly.

For more information about tlg2u's options see the man page in the
documentation directory distributed with the source code.


NOTE
----

Texts extracted with tlg2u can be edited with a polytonic typing aid like `Palamedes
<https://github.com/proteusx/Palamedes>`_ (Windows only) or with the `Vim editor
<http://www.vim.org/>`_ and the `polytonic Greek keymap
<http://www.vim.org/scripts/script.php?script_id=2743>`_.
There is also the Microsoft Greek polytonic keymap but it is
counterintuitive and awkward to use.
Also  have a look at the polytonic Greek `spell checker
<http://www.vim.org/scripts/script.php?script_id=2743>`_ for Vim. This
spell checker covers the complete vocabulary from Homeric through to
modern Greek.


SEE ALSO
--------


* `tlgu <http://tlgu.carmen.gr/>`_

* `Diogenes <https://d.iogen.es/>`_

* `The Digital Classicist Tools
  <https://wiki.digitalclassicist.org/Category:Tools>`_



.. vim: set ft=rst syntax=rst spell tw=75 fo-=l lbr wrap :

