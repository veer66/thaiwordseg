/*
 * decode.c - dictionary encoder for wi
 * Copyright (C) 2002 Vee Satayamas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMCHK(mem) if( (mem) == NULL ) { fprintf (stderr,"%s:%d: Out of memory.\n",__FILE__,__LINE__); exit(1); }

unsigned int *data;
FILE *fin;

void 
dump(unsigned int pos,int depth)
{
	unsigned int n=data[pos] & 0x7FFFFFFF;
	printf ("---------------------------------------------------------------------\n");
	printf ("Depth=%d\t\tPos = %d\t\t",depth,pos);
	printf ("N = %d\t\t",n);
	printf ("Type = %s\n",((data[pos] & 0x80000000)!=0) ?  "Table lookup" : "Binary search" );
	printf ("data[pos]=0x%X\n",data[pos]);
	printf ("Element-->\n");
	if ((data[pos] & 0x80000000) != 0) {
		int i;
		/* TABLE */
		for(i=pos+1;i<pos+0x100+1;i++) {
			if ((0xFF000000 & data[i]) != 0)  {
				char ch=((0xFF000000 & data[i]) >> 24);
				printf ("T d=%d ch=-%c-\tflag=%s\tnext=%d\tpos=%d\n",depth,ch,
						(0x00800000 & data[i])==0 ? "normal" : "terminator",
						data[i] & 0x007FFFFF,i);
				if ((data[i] & 0x007FFFFF)!=0)
					dump((data[i] & 0x007FFFFF)-1,depth+1);
			}
		}
	} else {
		/* list */
		unsigned int n = (data[pos] & 0x7FFFFFFF);
		int i;
		for (i=pos+1;i<pos+n+1;i++) {
			char ch=((0xFF000000 & data[i]) >> 24);
			printf ("L ch=-%c-\tflag=%s\tnext=%d\n",ch,
						(0x00800000 & data[i])==0 ? "normal" : "terminator",
						data[i] & 0x007FFFFF);
			if ((data[i] & 0x007FFFFF)!=0)
				dump((data[i] & 0x007FFFFF)-1,depth+1);
		}
	}
}



char temp[1024];

void 
regen(int pos,int depth)
{
//	printf ("---------------------------------------------------------------------\n");
//	printf ("Depth=%d\t\tPos = %d\t\t",depth,pos);
//	printf ("N = %d\t\t",n);
//	printf ("Type = %s\n",((data[pos] & 0x80000000)!=0) ?  "Table lookup" : "Binary search" );
//	printf ("data[pos]=0x%X\n",data[pos]);
//	printf ("Element-->\n");
	if ((data[pos] & 0x80000000) != 0) {
		int i;
		/* TABLE */
		for(i=pos+1;i<pos+0x100+1;i++) {
			if ((0xFF000000 & data[i]) != 0)  {
				char ch=((0xFF000000 & data[i]) >> 24);
				temp[depth] = ch; 
				if ((0x00800000 & data[i])!=0) 
				{
					temp[depth+1]='\0';
					printf ("%s\n",temp);
				}
			//	printf ("T ch=-%c-\tflag=%s\tnext=%d\n",ch,
			//			(0x00800000 & data[i])==0 ? "normal" : "terminator",
			//			data[i] & 0x007FFFFF);
				if ((data[i] & 0x007FFFFF)!=0)
					regen((data[i] & 0x007FFFFF)-1,depth+1);
			}
		}
	} else {
		/* list */
		unsigned int n = (data[pos] & 0x7FFFFFFF);
		int i;
		for (i=pos+1;i<pos+n+1;i++) {
			char ch=((0xFF000000 & data[i]) >> 24);
			temp[depth] = ch; 
			if ((0x00800000 & data[i])!=0) {
				temp[depth+1]='\0';
				printf ("%s\n",temp);
			}
			if ((data[i] & 0x007FFFFF)!=0)
				regen((data[i] & 0x007FFFFF)-1,depth+1);
		}
	}

}

int 
main (int argc,char** argv)
{
	int n;
	if (argc == 2) {
		if ((fin=fopen(argv[1],"r"))==NULL) {
			fprintf (stderr,"%s:%d: cannot open file \"%s\".\n",__FILE__,__LINE__,argv[1]);
			exit(1);
		}
	} else {
		fprintf (stderr,"Invalid argument.\n");
		exit(1);
	}
	fread(&n,sizeof(unsigned int),1,fin);
	MEMCHK(data=(unsigned int * )malloc(sizeof(unsigned int)*(n-1)));
	fread(data,sizeof(unsigned int),n-1,fin);
	printf ("---data[0]=%X n=%X---\n",(int)data[0],n);
	dump(0,0);
/* regen(0,0); */
	free(data);
	fclose(fin);
	return 0;
}
