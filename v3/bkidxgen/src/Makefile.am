AM_CFLAGS= -Wall -g $(GLIB2_CFLAGS) $(WORDCUT_CFLAGS) $(LATEXPARSER_CFLAGS)
bin_PROGRAMS=bkidxgen
bkidxgen_SOURCES=bigrams.c bkidxgen.c join.c
bkidxgen_LDFLAGS=$(GLIB2_LIBS) $(WORDCUT_LIBS) $(LATEXPARSER_LIBS) -lm
