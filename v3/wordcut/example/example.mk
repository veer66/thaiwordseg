CC=gcc 
	LDFLAGS=`pkg-config wordcut --libs`
CFLAGS=`pkg-config wordcut --cflags` -Wall -g
all : test1 wcdict_ex wcdictiter_ex wcdictiterpos_ex wcdictmap_ex wcwordcut_ex
test1 : test1.c
wcdict_ex : wcdict_ex.c
wcdictiter_ex : wcdictiter_ex.c
wcdictiterpos_ex : wcdictiterpos_ex.c
wcdictmap_ex : wcdictmap_ex.c
wcwordcut_ex : wcwordcut_ex.c
clean :
	rm -f test1 *.o core wcdict_ex wcdictiter_ex wcdictiterpos_ex wcdictmap_ex wcwordcut_ex
