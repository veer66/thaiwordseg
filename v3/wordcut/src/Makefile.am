SUBDIRS=util encoder
CFLAGS= -Wall -g $(GLIB2_CFLAGS)
INCLUDES= -I$(top_srcdir)/include
lib_LTLIBRARIES=libwordcut.la
libwordcut_la_SOURCES=wcdict.c wcwordunit.c wcdictmap.c wcwordcut.c
libwordcut_la_LIBADD=util/libwordcututil.la
libwordcut_la_LDFLAGS=$(GLIB2_LIBS)

#bin_PROGRAMS=my_wordcut
#my_wordcut_SOURCES=wcwordcut.c
#my_wordcut_LDADD=libwordcut.la
#my_wordcut_LDFLAGS=$(GLIB2_LIBS)
