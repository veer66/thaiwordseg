/* 
 *
 * Copyright (C) 2003 Vee Satayamas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or 
 * without modification, are permitted provided that the following 
 * conditions are met:
 * 
 *   1. Redistributions of source code must retain the above 
 *      copyright notice, this list of conditions and the following 
 *      disclaimer.
 *   2. Redistributions in binary form must reproduce the above 
 *      copyright notice, this list of conditions and the following 
 *      disclaimer in the documentation and/or other materials 
 *      provided with the distribution.
 *   3. The name of the author may not be used to endorse or 
 *      promote products derived from this software without specific 
 *      prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS 
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include <wordcut/xmalloc.h>

#include <wordcut/wordcut.h>

static void
dump_marker(int *marker,size_t size)
{
     size_t i;
     for(i=0;i<size;i++)  {
	  printf("%d ",marker[i]);
     }
     printf ("\n");
}

static int*
mk_morpho_chunk_tab(const char* str,size_t len)
{
     int i,buf,j,*tab,k,pre_stop,color=0,c,bak,x,prev;

     char *s,*p,*bs;
     const char* cp;
        
     s=(char *)xmalloc(sizeof(char)*(len+8));
     tab=(int *)malloc(sizeof(int)*(len+1));
     memset(tab,0,sizeof(int)*(len+1));
     bs=s;
     for(cp=str;*cp!='\0';cp++,s++) {
	  *s=*cp;
     }
     s[0]='\0';s[1]='\0';s[2]='\0';s[3]='\0';
     s[4]='\0';s[5]='\0';s[6]='\0';s[7]='\0';

     for(i=0,p=bs;*p!='\0';++p,++i) {
	  buf=0;
	  if ((*p=='à' || *p=='á') &&
	      (p[1]>='¡' && p[1]<='Î') &&
	      (p[2]>='Ô' && p[2]<='×')) {
	       /* to check Í and Â is probably worth */
	       if (p[3]>='è' && p[3]<='ë') {
		    if (buf<5) buf=5;                
	       } else {
		    if (buf<4) buf=4;
	       }
	  } else if (*p=='à' && p[1]!='¡' && p[2]=='ç') {
	       if(buf<4) buf=4;
	  } else if (*p=='¡' && p[1] =='ç') {
	       if(buf<2) buf=2;
	  }
	  if (buf<2 && *p>='à' && *p<='ä' && p[1]>='¡' && p[1]<='Î') {
	       buf=2;
	  }
	  if(buf<3 && p[1]=='Ñ') {
	       if (p[2]>='è' && p[2]<='ë') {
		    buf=4;
	       } else if (buf<2) {
		    buf=3;
	       }
	  }
        
	  if(buf<2 && p[1]>='è' && p[1]>='ë') {
	       buf=2;
	  }

	  if(buf<2 && p[0]>='è' && p[0]>='ë') {
	       buf=2;
	  }

	  if(buf<4 && p[3] =='ì') {
	       buf=4;
	  }

	  if(buf<4 && *p>='¡' && *p<='Î' && p[1]=='×') {
	       if(p[2]>='¡' && p[2]<='Î' ) {
		    if (buf<3) buf=3;
	       } else if (p[2]>='è' && p[2]<='ë'){
		    buf=4;
	       }
	  }

	  if(buf<2 && *p>='¡' && *p<='Î' && p[1]>='Ð' && p[1]<='Ù') {
	       buf=2;
	  }
/* Wait until next version ... try to merge »êÍ»ÍÒÂ by rules
   if(buf<3 && *p>='¡' && *p<='Î' && p[1]>='è' && p[1]>='ë' && 
   (*p!='º' || *p!='è')) {
   buf=3;
   }
*/
         
	  /* printf ("i=%d buf=%d *p=%c\n",i,buf,*p); */
        
	  ++color;
	  if(tab[i]!=0) { c=tab[i]; } else { c=color; }

	  if (buf>0 && buf+i<=len ) {
	       pre_stop=i+buf-1;
	       /* printf ("i=%d pre_stop=%d\n",i,pre_stop); */
	       for(k=i;k<pre_stop;k++) {
		    tab[k]=c;
	       }

	       if(tab[pre_stop]!=0) {
		    bak=tab[pre_stop];
		    for(j=pre_stop;j<len && tab[j]==bak;j++) {
			 tab[j]=c;
		    }
	       } else {
		    tab[pre_stop]=c;
	       }
	  }
     }

/*      dump_marker(tab,len+1); */

     free(bs);
     return tab;
}

static int
break_chunk(int *tab,size_t len,int start,int stop) {
     if (start<=stop && len) {
	  if(tab[stop]==tab[start] && tab[stop]!=0) {
	       if (start!=0 && tab[start]==tab[start-1]) {
		    return 1;
	       } else if (stop!=len-1 && tab[stop]==tab[stop+1]) {
		    return 1;
	       }	  
	  } else {
	       if(tab[start]!=0 && start!=0 && tab[start-1]==tab[start]) {
		    return 1;
	       } else if(tab[stop]!=0 && stop!=len-1 
			 && tab[stop+1]==tab[stop]) {
		    return 1;
	       }
	  }
     }
     return 0;
}

static int *
mk_morpho_chunk_graph(int *tab,size_t len) 
{
     int i,prev=0,start=(-1),stop=(-1);  
     int *map=(int *)xmalloc(sizeof(int)*len);
     for(i=0;i<len;i++) map[i]=(-1);
     for(i=0;i<len;i++) 
     {
	  if(prev!=tab[i])
	  {
	       stop=i;
	       if(start!=(-1) && prev!=0) {
		    map[start]=stop;
	       }
	       start=i;
	       prev=tab[i];
	  }
     }
     if(prev!=0) map[start]=len;
    
     return map;
}

int
wordcut_init(Wordcut *self,const char *dict_filename)
{
     /* return non-zero if not ok */
     return wordcut_dict_init(&(self->dict),dict_filename);
}


typedef struct
{
     int brk,unk,utok,tok,link;
}
PathInfo;


static void
dump_path(PathInfo *path,size_t len)
{
     int i;
     for(i=0;i<len;i++) {
	  printf ("[%d,link=%d,brk=%d,tok=%d]\n",i,path[i].link,path[i].brk,path[i].tok);        
     }
     printf ("\n");
}



void
wordcut_cut(Wordcut *self,const char *str,WordcutResult *result)
{
#define DEFAULT_GRAPH_SIZE 4096
    
     /* build tree */
     size_t len=strlen(str);
     int i,j,walk_result;
     WordcutDict *dict=&self->dict;
     WordcutDictNode node;
     size_t graph_size=len+len;
     int *idx,*graph,gcount,c,*chunk_tab,*chunk_graph,cc;
     PathInfo *path;
     int single_lead,offset;
     WordcutResult *tmp_result;
     int *marker;

     if (len > DEFAULT_GRAPH_SIZE) {
	  graph_size=len+len;
     } else {
	  graph_size=DEFAULT_GRAPH_SIZE;
     }
        
     idx=(int *)xmalloc(sizeof(int)*(len+1));
     graph=(int *)xmalloc(sizeof(int)*graph_size);
     path=(PathInfo *)xmalloc(sizeof(PathInfo)*(len+1));
     tmp_result=(WordcutResult *)xmalloc(sizeof(WordcutResult));

     chunk_tab=mk_morpho_chunk_tab(str,len);
     chunk_graph=mk_morpho_chunk_graph(chunk_tab,len);
    
     gcount=0;
     idx[0]=0;
     for(i=0;i<len;i++)
     {
	  wordcut_dict_node(&node,dict);
	  walk_result=0;
	  for(j=i;walk_result!=WORDCUT_DICT_WALK_FAIL && j<len;j++) {
/*            printf ("!!! i=%d j=%d p1=%d p2=%d status=%d size=%d ch=%c\n",
	      i,j,node.p1,node.p2,node.status,node.size,str[j]); */
	       walk_result=wordcut_dict_walk(&node,(unsigned char)str[j]);
/*
  printf ("@@@ p1=%d p2=%d status=%d size=%d word_result=%d\n",
  node.p1,node.p2,node.status,node.size,
  walk_result);*/
	       if (walk_result==WORDCUT_DICT_WALK_COMPLETE) {
		    if(gcount==graph_size) {
			 graph_size=graph_size+graph_size;
			 graph=(int *)xrealloc(graph,graph_size);
		    }
		    graph[gcount]=j+1;
		    gcount++;
	       }            
	  }
	  idx[i+1]=gcount;
     }

     path[len].link = (-1);
     path[len].unk = 0;
     path[len].utok = 0;
     path[len].tok = 0;
     path[len].brk = 0;


    
     for(i=len-1;i>=0;i--) {
	  int unk_p;
	  if (chunk_graph[i]!=(-1)) {
	       unk_p=chunk_graph[i];
	  } else {
	       unk_p=i+1;
	  }
        
	  path[i].link=unk_p;
	  path[i].tok = path[unk_p].tok + 1;
	  path[i].unk = path[unk_p].unk + 1;
	  path[i].brk = path[unk_p].brk + break_chunk(chunk_tab,len,i,unk_p-1);
        
	  for(j=idx[i+1]-1;j>=idx[i];j--) {
	       int brk,tok,unk;
	       c=graph[j];
	       tok=path[c].tok+1;
	       unk=path[c].unk;
	       brk=path[c].brk+break_chunk(chunk_tab,len,i,path[c].link-1);

	       /* printf ("I=%d\tC=%d\n",i,c); */
            
	       if(brk<path[i].brk ||
		  (brk==path[i].brk &
              
		   (unk<path[i].unk ||
		    (unk==path[i].unk && tok<path[i].tok ))))
            
/*            if  (unk<path[i].unk ||
              (unk==path[i].unk && tok<path[i].tok ))*/
            
	       {
                
		    path[i].link=c;
		    path[i].tok=tok;
		    path[i].unk=unk; 
	       }
        
	  }
/*         printf ("i=%d\n",i);dump_path(path,len);  */
     }


    
     result->start=(int *)xmalloc(sizeof(int)*len);
     result->offset=(int *)xmalloc(sizeof(int)*len);

     tmp_result->start=(int *)xmalloc(sizeof(int)*len);
     tmp_result->offset=(int *)xmalloc(sizeof(int)*len);

     marker=(int *)xmalloc(sizeof(int)*len);

     memset(marker,0,sizeof(int)*len);

     single_lead = 0;
    
     for(c=0,j=0;j!=len;j=i) {
	  i=path[j].link;
	  offset=i-j;        	
	  tmp_result->start[c]=j;
	  tmp_result->offset[c]=i-j;            
	  c++;
     }
 
     for(i=c-1;i>=0;i--) {
	  if(tmp_result->offset[i]==1) {
	       if(i!=0 && (i+1==c || marker[i+1]==0)) {
		    marker[i]=1;
	       } else {
		    if (c>1 && marker[i+1]==0) {
			 marker[i]=1;
		    }
	       }
	  }
     }

     cc=0;
     if(c>1 && (marker[0]==1 || marker[1]==1)) {
	  result->start[0]=tmp_result->start[0];
	  result->offset[0]=tmp_result->offset[0] + tmp_result->offset[1];
	  cc=1;
	  i=2;
     } else {
	  result->start[0]=tmp_result->start[0];
	  result->offset[0]=tmp_result->offset[0];
	  cc=1;
	  i=1;
     }

     for(;i<c;i++) {
	  if (marker[i]!=1) {
	       if (i+1<c && marker[i+1]==1) {
	       result->start[cc]=tmp_result->start[i];
		    result->offset[cc]=tmp_result->offset[i]+tmp_result->offset[i+1];
		    cc++;
	       }  else {
		    result->start[cc]=tmp_result->start[i];
		    result->offset[cc]=tmp_result->offset[i];
		    cc++;
	       }
	  }
     }   

     result->count=cc;
    
     result->str=(char *)xmalloc(sizeof(char)*(len+1));
     strcpy(result->str,str);
    
     free(idx);
     free(path);
     free(graph);
     free(chunk_tab);
     free(chunk_graph);
     free(tmp_result->start);
     free(tmp_result->offset);
     free(tmp_result);
     free(marker);
}

void
wordcut_close(Wordcut *self)
{
     wordcut_dict_close(&(self->dict));
}

void
wordcut_result_close(WordcutResult *self)
{
     free(self->start);
     free(self->offset);
     free(self->str);
}
