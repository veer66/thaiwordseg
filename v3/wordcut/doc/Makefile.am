EXTRA_DIST=wordcut.th.tex

showdvi : wordcut.dvi
	xdvi $<

wordcut.tex : wordcut.th.tex
	swath -f latex < $< > $@

wordcut.dvi : wordcut.tex
	latex $<

cleantex : 
	rm -f wordcut.aux wordcut.dvi wordcut.tex wordcut.log

