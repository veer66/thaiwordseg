all : wordcut.dvi

showdvi : wordcut.dvi
	xdvi $<

showpdf : wordcut.pdf
	acroread $<

wordcut.pdf : wordcut.dvi
	dvipdf $<

wordcut.tex : wordcut.th.tex
	swath -f latex < wordcut.th.tex > wordcut.tex

wordcut.dvi : wordcut.tex
	latex $<
	latex $<
	makeindex -T  `echo $< | sed 's/\.tex$$//g'`
	latex $<

clean :
	rm -f *.dvi *.log *.aux *.idx *.ind *.ilg wordcut.tex *.toc
