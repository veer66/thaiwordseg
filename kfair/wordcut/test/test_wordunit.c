#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<wordcut/wcwordunit.h>
int main()
{
	const char* str="มาก";
	int len=strlen(str);
	int i,j;
	WcWordunitMap *map;
	map=wc_wordunit_find(str,len);
	for(i=0;i<map->len;i++) {
		printf ("i=%d\tassoc=%d\n",i,
			wc_wordunit_map_assoc(map,i));
	}
	printf ("*********\n");
	for(i=0;i<6;i++) {
		for(j=i;j<map->len;j++) {
			printf ("start=%d\tstop=%d\tbreak=%c\n",
				i,j,wc_wordunit_map_break(map,i,j) ? 'T' : 'F' );
		}
	}
	return 0;
}
