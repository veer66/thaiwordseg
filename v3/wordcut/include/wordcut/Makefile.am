wordcutincludedir=$(includedir)/wordcut
wordcutinclude_HEADERS= wccommon.h wcdict.h wcstring.h wcwordunit.h \
	wcsplit.h wcwordcut.h wcpath.h wcdictmap.h \
	wcsymhash.h

show : wordcut.dvi
	xdvi $<

wordcut.dvi : wordcut.tex
	latex wordcut
