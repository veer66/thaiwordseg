SUBDIRS=util encoder
CFLAGS= -Wall -g 
bin_PROGRAMS=wcwordcut
INCLUDES= -I$(top_srcdir)/include
lib_LTLIBRARIES=libwordcut.la
libwordcut_la_SOURCES=libwordcut.c
libwordcut_la_LIBADD=util/libwordcututil.la
wcwordcut_SOURCES=wcdict.c
