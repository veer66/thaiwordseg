/*
 * encode.c - dictionary encoder for wi
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
#include <assert.h>

typedef int boolean;
#define FALSE	0
#define TRUE    !FALSE

#define MEMCHK(mem) if( (mem) == NULL ) { fprintf (stderr,"%s:%d: Out of memory.\n",__FILE__,__LINE__); exit(1); }

#define TABLE_SIZE 0x100


FILE *fp,*fout;
int tab_or_list;

struct tab ;
struct node;

typedef struct node {
	struct node *next,*depth;
	boolean terminator;
 	char ch;
} Node;

Node *root;


Node*
node_new() 
{
	Node *node;
	MEMCHK(node=(Node *)malloc(sizeof(Node)));
	node->next=NULL;
	node->depth=NULL;
	return node;
}

Node**
addchar(Node **node,char ch,boolean terminator)
{
	while (*node!=NULL && (*node)->ch != ch) {
		node=&((*node)->next);
	}

	if (*node==NULL) {
		*node=node_new();
		(*node)->ch=ch;
		(*node)->terminator=terminator;
	} else {
		assert (node[0]->ch == ch);
		(*node)->terminator|=terminator;
	}
	return &((*node)->depth);
}

char *words[100000];
int count;
int c;
char string[1024];

void regen2(Node *node,int n);

void
addword (const char *word)
{
	const char *pch;
	Node **node=&root;
	MEMCHK(words[count++]=(char *)strdup(word));
	for (pch=word;*pch!='\0';pch++) {
		node=addchar(node,*pch,(*(pch+1)=='\0'));
	}
	c=0;
#ifdef DEBUG
	regen2(root,0);
#endif
}

void regen(Node *node,int n);
void 
regen2(Node *node,int n)
{
	if (node != NULL) {
		string[n]=node->ch;
		if (node->terminator) {
			string[n+1]='\0';
/*			printf ("count=%d c=%d str=%s words=%s\n",count,c,string,words[c]); */
			if (strcmp(words[c],string)!=0) {
				printf ("*** count=%d c=%d str=%s words=%s\n",count,c,string,words[c]);
				regen(root,0);
				exit(1);
			}
			c++;
		}
		regen2(node->depth,n+1);
		regen2(node->next,n);
	}
}

void 
dumptree(Node* node)
{
	printf (" -%c %c - ",node->ch,node->terminator ? 'T' : 'N' );
	if(node->depth!=NULL) {
		printf ("D\t");
		dumptree(node->depth);
	}
	if(node->next!=NULL) {
		printf ("N\n");
		dumptree(node->next);
	}
}


void 
sel_sort(Node **a,unsigned int n)
{
	unsigned int i,j,k;
	Node *t;
	for (i=0;i<n-1;i++) {
		k=i;
		for (j=i+1;j<n;j++) {
			if(a[k]->ch > a[j]->ch)  {
				k=j;
			}
		}
		if (k!=i) {
			t=a[k];
			a[k]=a[i];
			a[i]=t;
		}
	}
}

unsigned int
savetree(Node *tree,unsigned int pos)
{
	unsigned int count=0;
	unsigned int header=0;
	unsigned int spos=pos;
	unsigned int data;
	Node *temp;
	printf ("------------------------------------------------\n");
	printf ("pos=%d\n",pos);
	for (temp=tree;temp!=NULL;temp=temp->next) {
		count++;
	}
	if ((count & 0x10000000) != 0) {
		fprintf (stderr,"%s:%d: counting overflow.\n",__FILE__,__LINE__);
		exit(1);
	}
	header|=(count & 0x7FFFFFFF);
	fseek(fout,spos*sizeof(int),SEEK_SET);
	if (count > tab_or_list) {
		header|=0x80000000;
		printf ("T Header=0x%X\n",header);
		fwrite(&header,sizeof(int),1,fout);
		pos+=TABLE_SIZE+1;
		for (temp=tree;temp!=NULL;temp=temp->next) {
			unsigned int p=((unsigned char)temp->ch+1)*sizeof(unsigned int);
			data=0;
			data|=(((unsigned char)temp->ch)<<24);
			data|=(temp->terminator<<23);
			if (temp->depth!=NULL) {
				data|=(pos & 0x007FFFFF);
				printf ("inner pos=%d\n",pos);
				pos=savetree(temp->depth,pos);
			}
			printf ("Wi\n");
			fseek(fout,spos*sizeof(int)+p,SEEK_SET);
			
			fwrite(&data,sizeof(int),1,fout);
		}
	} else {
		Node *sorted_list[TABLE_SIZE];
		unsigned int size=0;
		int i;
		printf ("L Header=0x%X\n",header);
		fwrite(&header,sizeof(int),1,fout);

		for (temp=tree;temp!=NULL;temp=temp->next) {
			sorted_list[size++] = temp;
		}		
		pos+=size+1;
		sel_sort((Node **)&sorted_list,size);
		for (i=0;i<size;i++) {
			data=0;
			data|=(((unsigned char)sorted_list[i]->ch)<<24);
			data|=(sorted_list[i]->terminator<<23);
			if(sorted_list[i]->depth!=NULL) 
			{
				data|=(pos & 0x007FFFFF);
				printf ("inner pos=%d\t\tsize=%d\n",pos,size);
				pos=savetree(sorted_list[i]->depth,pos);
			}
			fseek(fout,(spos+i+1)*sizeof(int),SEEK_SET);
			fwrite(&data,sizeof(int),1,fout);
			printf ("Wi\n");
		}
	}
	return pos;
}


void 
regen(Node *node,int n)
{
	if (node != NULL) {
		string[n]=node->ch;
		if (node->terminator) {
			string[n+1]='\0';
			printf ("%s\n",string);
		}
		regen(node->depth,n+1);
		regen(node->next,n);
	}
}

int 
main (int argc,char **argv)
{
#define BUFFER_SIZE		1024
	char buffer[BUFFER_SIZE];
	char *p;
	unsigned int n,count=1;
	count=0;
	tab_or_list = 30;
	root=NULL;
	if (argc == 3)  {
		fp = fopen (argv[1],"r");
		if (fp==NULL) {
			fprintf (stderr,"%s:%d: Can't open file \"%s\"\n",__FILE__
						,__LINE__,argv[1]);
		}
		fout=fopen(argv[2],"w");
		if (fout==NULL) {
			fprintf (stderr,"%s:%d: Can't open file \"%s\"\n",__FILE__
						,__LINE__,argv[1]);
		}
	} else if (argc == 1) {
		fp = stdin;
		fout = stdout;
	} else {
		fprintf (stderr,"Invalid argument.\n");
		return 1;
	}
		
	while (fgets(buffer,BUFFER_SIZE,fp)!=NULL) {
		p=strtok(buffer," \n\r\t");
		if (p!=NULL) {
			addword(p);
			count++;
//			printf ("%d\n",count++);
		}
	}

	if (fp!=stdin) fclose(fp);
	n=savetree(root,1); 
	printf ("n=0x%X\n",n*sizeof(int));
	fseek(fout,0,SEEK_SET);
	fwrite (&n,sizeof(unsigned int),1,fout);
	//regen(root,0);
	if (fout!=stdout)  { 
		fclose(fout);
	}
	return 0;
}
