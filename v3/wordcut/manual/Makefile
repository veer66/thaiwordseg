all : wordcut.dvi

showpdf : wordcut.pdf
	acroread $<

wordcut.pdf : wordcut.dvi
	dvipdf $<

showdvi : wordcut.dvi
	xdvi $<

wordcut.dvi : wordcut.tex ref_c_api_wu.tex ref_c_api_wordcut.tex ref_c_api_dict.tex ref_c_api_basic.tex design.tex algorithm.tex acknownledge.tex 
	latex wordcut
	latex wordcut
	bibtex wordcut
	makeindex -T wordcut
	latex wordcut
	latex wordcut

acknownledge.tex : acknownledge.th.tex
	swath -f latex < $< > $@

algorithm.tex : algorithm.th.tex
	swath -f latex < $< > $@

design.tex : design.th.tex
	swath -f latex < $< > $@

ref_c_api_basic.tex : ref_c_api_basic.th.tex
	swath -f latex < $< > $@

ref_c_api_dict.tex : ref_c_api_dict.th.tex
	swath -f latex < $< > $@

ref_c_api_wordcut.tex : ref_c_api_wordcut.th.tex
	swath -f latex < $< > $@

ref_c_api_wu.tex : ref_c_api_wu.th.tex
	swath -f latex < $< > $@

wordcut.tex : wordcut.th.tex
	swath -f latex < $< > $@
clean:
	rm -f wordcut.tex ref_c_api_wu.tex ref_c_api_wordcut.tex ref_c_api_dict.tex ref_c_api_basic.tex design.tex algorithm.tex acknownledge.tex 
	rm -f *.dvi *.pdf *.aux *.lof *.log *.idx *.ind *.ilg *.toc *.bbl
update:
	./autottex.sh wordcut > Makefile

distclean : clean
	rm -f *~ *.tar.gz

dist : distclean
	rm -rf wordcut-20030304
	mkdir wordcut-20030304
	for x in * ;do if [ -f $$x ];then cp $$x wordcut-20030304 ;fi ;done
	tar cvvzf wordcut-20030304.tar.gz wordcut-20030304
	rm -rf wordcut-20030304
