INCLUDES= -I$(top_srcdir)

libwordcut_HEADERS=wordcut.h wordcut_dict.h xmalloc.h
libwordcutdir=$(includedir)/wordcut

bin_PROGRAMS=wordcut_mkdict wordcut

lib_LTLIBRARIES=libwordcut.la
libwordcut_la_SOURCES=wordcut_dict.c wordcut.c xmalloc.c

wordcut_mkdict_SOURCES=wordcut_mkdict.c
wordcut_SOURCES=wordcut_cli.c
wordcut_LDADD=libwordcut.la
