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
#include "wordcut_dict.h"

typedef struct
{
    size_t count;
    int *start;
    int *offset;
    char *str;
} WordcutResult;

typedef struct
{
    WordcutDict dict;
} Wordcut; 

int
wordcut_init(Wordcut *self,const char *dict_filename)
{
    /* return non-zero if not ok */
    return wordcut_dict_init(&(self->dict),dict_filename);
}

void
wordcut_cut(Wordcut *self,const char *str,WordcutResult *result)
{
    /* build tree */
    size_t len=strlen(str);
    int i,j,walk_result;
    WordcutDict *dict=&self->dict;
    WordcutNode node;
    
    for(i=0;i<len;i++)
    {
        wordcut_dict_node(&node,dict);
        walk_result=0;
        for(j=i;walk_result!=WORDCUT_DICT_WALK_FAIL && j<len;j++)
        {
            walk_result=wordcut_dict_walk(&node,(unsigned char)str[j]);            
        }
    }
}


void
wordcut_close(Wordcut *self)
{
    wordcut_dict_close(&(self->dict));
}


int main (int argc,char** argv) {
    
    return 0;
}
