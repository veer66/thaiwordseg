CFLAGS= -Wall -g 
INCLUDES= -I$(top_srcdir)/include
bin_PROGRAMS=wcencoder
wcencoder_SOURCES=wcencoder.c dictionary.c strcoder.c \
	charcoder.c trienode.c posarray.c trie.c
wcencoder_LDADD=${top_builddir}/src/util/libwordcututil.la
EXTRA_DIST=trienode.h dictionary.h word.h strcoder.h  \
	charcoder.h trienode.h posarray.h \
	trie.h 

