CC=gcc
LDFLAGS=`pkg-config wcdict --libs`
CFLAGS=`pkg-config wcdict --cflags` -Wall -g
all : test1
test1 : test1.c
clean :
	rm -f test1 test1.o core
