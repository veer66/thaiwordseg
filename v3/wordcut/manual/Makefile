all : wordcut.dvi

showdvi : wordcut.dvi
	xdvi $<

wordcut.dvi : wordcut.tex
	latex $<
	latex $<
	makeindex $<
	latex $<

clean :
	rm -f *.dvi *.log *.aux *.idx *.ind *.ilg
