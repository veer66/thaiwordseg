all : wordcut.dvi

showdvi : wordcut.dvi
	xdvi $<

wordcut.tex : wordcut.th.tex
	swath -f latex < wordcut.th.tex > wordcut.tex

wordcut.dvi : wordcut.tex
	latex $<
	latex $<
	makeindex `echo $< | sed 's/tex$$/idx/g'`
	latex $<

clean :
	rm -f *.dvi *.log *.aux *.idx *.ind *.ilg wordcut.tex *.toc
