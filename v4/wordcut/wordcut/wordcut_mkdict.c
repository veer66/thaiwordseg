/*
 * Copyright (c) 2003 , Vee Satayamas
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided
 * that the following conditions are met:
 *
 * - Redistributions of source code must retain the above
 *   copyright notice, this list of conditions and the
 *   following disclaimer.
 * - Redistributions in binary form must reproduce the
 *   above copyright notice, this list of conditions and
 *   the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * - Neither the name of the <ORGANIZATION> nor the names
 *   of its contributors may be used to endorse or promote
 *   products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<assert.h>
#include<fcntl.h>
#include<unistd.h>


#define TAB_SIZE 0x100

typedef struct trienode_t{
    int complete;
    uint32_t data;
    struct linktab_t* linktab;
} TrieNode;

typedef struct linktab_t {
    size_t size;
    TrieNode *link[TAB_SIZE];
} LinkTab;



TrieNode *root;

int verbose=0;


TrieNode* new_trienode() {
    TrieNode *tmp;
    tmp=malloc(sizeof(TrieNode));
    if (tmp==NULL) {
        fprintf (stderr,"%s:%d: Could not allocate memory for trie node.\n",__FILE__,__LINE__);
        exit(1);
    }
    tmp->complete=0;
    tmp->linktab=NULL;
    return tmp;
}

void trie_init() {
    root=new_trienode();
};

LinkTab* new_linktab() {
    LinkTab *tmp;
    tmp=malloc(sizeof(LinkTab));
    if (tmp==NULL) {
        fprintf (stderr,"%s:%d: Could not allocate memory for link table.\n",__FILE__,__LINE__);
        exit(1);
    }
    tmp->size=0;
    memset(tmp->link,0,sizeof(tmp->link));
    return tmp;
}   

void trie_add(const char *str) {
    uint8_t *key=(uint8_t *)str , *i;    
    TrieNode *node=root;
    
    for(i=key;*i!=(uint8_t)'\0';i++) {
        /* looking for exist link */
        if (node->linktab!=NULL && node->linktab->link[*i]!=NULL) {
            node=node->linktab->link[*i];
        } else {
            if(node->linktab==NULL) {
                node->linktab=new_linktab();
            } 
            node->linktab->link[*i]=new_trienode();
            node->linktab->size++;
            node=node->linktab->link[*i];
        }
    }
    node->complete=1;
}

void load_dict(const char* filename) {
    FILE *fp;
    if((fp=fopen(filename,"r"))==NULL) {
        fprintf (stderr,"%s:%d: Could not open dictionary.\n",__FILE__,__LINE__);
        exit(1);
    }
#define MAX_BUF 0x400
    char buffer[MAX_BUF];
    while(fgets(buffer,MAX_BUF,fp)!=NULL) {
        char *tok_buffer;
        tok_buffer=strtok(buffer,"\n\r\t ");
        trie_add(tok_buffer);
        
    }
    fclose(fp);
}


char dump_buffer[1024];

void dump_(TrieNode *node,int c) {
    if (node==NULL) return;

    if (node->complete) { 
        int i;

        for(i=0;i<c;i++) {
            printf ("%c",dump_buffer[i]);
            
        }
        printf ("\n");
    }
     
    if (node->linktab!=NULL && node->linktab->size>0) {
        int i;
        LinkTab *tab=node->linktab;
        for(i=0;i<TAB_SIZE;i++) {
            if (tab->link[i]!=NULL) {
                dump_buffer[c]=(char)i;
                dump_(tab->link[i],c+1);
            }
        }
    }
}

void dump() {
    dump_(root,0);
}


#define OUTSIZE 1000000

uint8_t outb[OUTSIZE];

#define LINK_SIZE 3


unsigned int encode_(TrieNode *node,int start) {
    
    int cpos;
    uint8_t header=0,size;

    assert (node!=NULL);
    
    cpos=start;

    if (node->complete) {
        header=(header|1);
    }

    if (node->linktab==NULL || node->linktab->size<=1) {
        header=(header|2);
    }

    if (verbose) printf ("Header=%d\n",header);
    
    outb[cpos]=header;
    cpos++;


#ifdef ENABLE_DATA
    if (node->complete) {
        /* skip data */
        cpos=cpos+4;
    }
#endif

    if (node->linktab==NULL || node->linktab->size==0) {
        if (verbose) printf ("!!! NULL NODE\n");
        size = 0;
        outb[cpos]=0;
        cpos++;
        return cpos;
    } else if (node->linktab->size==1) {
        int i,flag=1,c=0,j;
        unsigned int lpos,xpos;
        uint8_t buffer[1024];
        int k;
        TrieNode* next;

        if (verbose) printf ("!!! COMPRESS NODE\n");
        
        while(flag) {
            for(i=0;i<TAB_SIZE;i++) {
                
                if (node->linktab->link[i]!=NULL) {
                    next=node->linktab->link[i];                    
                    
                    buffer[c]=(uint8_t)i;
                    c++;
                    
                    if( next->complete || next->linktab->size != 1 ) {
                        node=next;
                        flag=0;
                        break;
                    } else {
                        node=next;
                    }
                }
                
            }
        }

        
        assert (c>0);

        /* write size */
        outb[cpos]=(uint8_t)c;
        cpos++;

        for(j=0;j<c;j++) {
            outb[cpos]=buffer[j];
            cpos++;
        }

        lpos=cpos+LINK_SIZE;
                        

        xpos=encode_(node,lpos);

        
        if(lpos > 0x00FFFFFF) {
            fprintf(stderr,"Dictionary size is too big\n");
            exit(1);
        }    

        /* Little Endian */        
        for(k=0;k<LINK_SIZE;k++) {
            outb[cpos]=(uint8_t)(lpos & 0xFF);
            cpos++;
            lpos = (lpos >> 8);
        }
        
        return xpos;
    } else {
        unsigned int tsize,i,lpos,xpos;
        int k;

        if (verbose) printf ("!!! NORMAL NODE\n");
        
        assert(node->linktab->size>1);
        tsize=node->linktab->size;
        if (verbose) printf ("### cpos=%d size=%d\n",cpos,tsize);
        outb[cpos] = (0xFF & tsize);
        cpos++;
        lpos=cpos+(tsize*(LINK_SIZE+1));
        for(i=0;i<TAB_SIZE;i++) {
            if(node->linktab->link[i]!=NULL) {
                outb[cpos]=(uint8_t)i;
                cpos++;                
                xpos=encode_(node->linktab->link[i],lpos);
                /* write */
                for(k=0;k<LINK_SIZE;k++) {
                    outb[cpos]=(uint8_t)(lpos & 0xFF);
                    cpos++;
                    lpos = (lpos >> 8);
                }
                lpos=xpos;
            }
        }
        return lpos;
    }
}

void encode(int *size) {
    *size=encode_(root,1);
}

int main (int argc,char** argv) {
    int size,fd;
    char *fn1,*fn2;
    ssize_t ret,x=0;

    
    if (argc<3) {
        fprintf (stderr,"Usage: encoder (-v) <plain dict> <encoded dict>\n");
        exit(1);
    }

    if (argc==4) {
        if (strcmp(argv[1],"-v")==0) {
            verbose=1;
        } else {
            fprintf (stderr,"Usage: encoder (-v) <plain dict> <encoded dict>\n");
            exit(1);
        }
        fn1=argv[2];
        fn2=argv[3];
    } else {
        fn1=argv[1];
        fn2=argv[2];
    }
    
    trie_init();
    load_dict(fn1);
    /* dump();
       printf ("\n"); */
    
    encode(&size);
    
    /* printf ("Size = %d\n",size); */
/*
    for(i=0;i<size;i++) {
        printf ("#%d/%d ",i,outb[i]);
    }
    printf ("\n");
*/
  
    fd=open(fn2,O_WRONLY | O_CREAT );
    while(size>0) {
        ret=write(fd,outb+x,size);
        if (ret==(-1)) {
            fprintf (stderr,"Could not write result.\n");
            exit(1);
        }
        size=size-ret;
        x=ret;
    }
    close(fd);
    return 0;
}
