#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int main ()
{
	FILE *fp;
	int i,j,k;
	char *x;
	char buffer[1024];
	char *t[100000];
	int c=0;
	if ((fp=fopen("test3.txt","r"))==NULL) {
		fprintf (stderr,"can't open file for read");
		exit(1);
	}

	while (fgets(buffer,1024,fp)!=NULL) {
		t[c++] = (char *)strdup(buffer);
	}

	for (i=0;i<c-1;i++) {
		k=i;
		for (j=i+1;j<c;j++) {
			if (strcmp(t[k],t[j])>0) k=j;
		}
		if (k!=i) { x=t[i]; t[i]=t[k]; t[k]=x; }
	}
	for (i=0;i<c;i++) {
		printf ("%s",t[i]);
	}

	fclose(fp);
	return 0;
}
