CC=gcc
LDFLAGS=`pkg-config wordcut --libs`
CFLAGS=`pkg-config wordcut --cflags` -Wall -g
all : test1
test1 : test1.c
clean :
	rm -f test1 test1.o core
