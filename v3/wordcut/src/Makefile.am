SUBDIRS=util encoder
AM_CFLAGS= -Wall -g
INCLUDES= -I$(top_srcdir)/include
lib_LTLIBRARIES=libwordcut.la
libwordcut_la_SOURCES=wcdict.c wcwordunit.c wcdictmap.c #wcwordcut.c
libwordcut_la_LIBADD=util/libwordcututil.la

#bin_PROGRAMS=wordcut
#wordcut_SOURCES=wordcut.c
#wordcut_LDADD=libwordcut.la

#bin_PROGRAMS=dictmap
#dictmap_SOURCES=wcdictmap.c 
#dictmap_LDADD=libwordcut.la
