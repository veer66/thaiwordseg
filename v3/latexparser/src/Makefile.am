BUILT_SOURCES=latex.h
INCLUDES= -I$(top_srcdir)/include
CFLAGS=$(GLIB2_CFLAGS) -Wall -g
latex.h : latex.l
	$(LEX) -t $< > $@

EXTRA_DIST=latex.l
bin_PROGRAMS=latexecho
latexecho_SOURCES=latexecho.c
latexecho_LDFLAGS=$(LEXLIB)  $(GLIB2_LIBS)
latexecho_LDADD=liblatexparser.la

lib_LTLIBRARIES=liblatexparser.la
liblatexparser_la_SOURCES=latexparser.c 
liblatexparser_la_LDFLAGS=$(LEXLIB) $(GLIB2_LIBS)
