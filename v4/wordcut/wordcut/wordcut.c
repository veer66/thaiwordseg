/*
 * Copyright (c) 2003 Vee Satayamas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 4. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Vee Satayamas.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include <wordcut/xmalloc.h>

#include <wordcut/wordcut.h>


int
wordcut_init(Wordcut *self,const char *dict_filename)
{
    /* return non-zero if not ok */
    return wordcut_dict_init(&(self->dict),dict_filename);
}


typedef struct
{
    int unk,tok,link;
}
PathInfo;

void
wordcut_cut(Wordcut *self,const char *str,WordcutResult *result)
{
    /* build tree */
    size_t len=strlen(str);
    int i,j,walk_result;
    WordcutDict *dict=&self->dict;
    WordcutDictNode node;
    size_t graph_size=len+len;
    int *idx,*graph,gcount,c;
    PathInfo *path;
    
    idx=(int *)xmalloc(sizeof(int)*(len+1));
    graph=(int *)xmalloc(sizeof(int)*graph_size);
    path=(PathInfo *)xmalloc(sizeof(PathInfo)*(len+1));
    
    gcount=0;
    idx[0]=0;
    for(i=0;i<len;i++)
    {
        wordcut_dict_node(&node,dict);
        walk_result=0;
        for(j=i;walk_result!=WORDCUT_DICT_WALK_FAIL && j<len;j++) {
            walk_result=wordcut_dict_walk(&node,(unsigned char)str[j]);
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
    path[len].tok = 0;
    
    for(i=len;i>=0;i--) {
        path[i].link=i;
        path[i].tok = path[i+1].tok + 1;
        path[i].unk = path[i+1].unk + 1;

        for(j=idx[i];j<idx[i+1];j++) {
            int tok,unk;
            c=graph[j];
            tok=path[c].tok+1;
            unk=path[c].unk;
            if(unk<path[i].unk) {
                path[i].link=c;
                path[i].tok=tok;
                path[i].unk=unk;
            } else if (unk==path[i].unk && tok<path[i].tok) {                
                path[i].link=c;
                path[i].tok=tok;
                path[i].unk=unk;
            }
        }        
    }


    
    result->start=(int *)malloc(sizeof(int)*len);
    result->offset=(int *)malloc(sizeof(int)*len);
    
    for(c=0,j=0;j!=len;j=i,c++) {
        i=path[j].link;
        result->start[c]=j;
        result->offset[c]=i-j;
    }
    result->count=c;
    
    result->str=(char *)xmalloc(sizeof(char)*(len+1));
    strcpy(result->str,str);
    
    free(idx);
    free(path);
    free(graph);
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

