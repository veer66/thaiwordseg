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


#include<string.h>
#include<assert.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<wordcut/wordcut_dict.h>

int wordcut_dict_init(WordcutDict *self,const char *filename) {
    int fd=open(filename,O_RDONLY);
    struct stat st;
    uint8_t *data;
    off_t size;
    int x=0;
    if (fd==(-1)) return -1;
    if(fstat(fd,&st)==(-1)) return -1;
    size=st.st_size;
    self->size=(size_t)size;
    data=(uint8_t *)malloc(self->size);
    while(size>0) {
        x=read(fd,data+x,size);
        if (x==(-1)) {
            fprintf (stderr,"Could not read dictionary\n");
            exit(1);
        }
        size=size-x;
    }
    self->data=data;
    close(fd);
    return 0;
}

void wordcut_dict_close(WordcutDict *self) {
    free(self->data);
}

void wordcut_dict_node(WordcutDictNode *self,WordcutDict *dict) {
    self->p1=1;
    self->p2=0;
    self->data=dict->data;
    self->status= (self->data[0] & 2);
    self->size=self->data[2];
}

int wordcut_dict_walk(WordcutDictNode *node,unsigned char key) {
    uint8_t *data=node->data;
    int p1=node->p1,p2=node->p2;
    if (node->status & 2) {
        /* suffix */
        if (data[p1+2+p2]==(uint8_t)key) {
            p2++;
            if (node->size==p2) {
                /* jump */
                p1=(data[p1+2+p2] |  (data[p1+3+p2] << 8) | (data[p1+4+p2] << 16));
                node->status=(data[p1] & 2);
                node->p2=0;
                node->size = data[p1+1];
                node->p1=p1;
                return ((data[p1] & 1))+WORDCUT_DICT_WALK_OK;
            } else {
                return WORDCUT_DICT_WALK_OK;
            }
        } else {
            return WORDCUT_DICT_WALK_FAIL;
        }
    } else {
        int l,r,m;
        uint8_t t;
        l=0;
        r=node->size-1;
        while (l<=r) {
            m=(l+r)/2;
            t=data[p1+2+m*4];
            if ((uint8_t)key < t) {
                r=m-1;
            } else if((uint8_t)key > t) {
                l=m+1;
            } else {
                p1=(data[p1+3+m*4] |  (data[p1+4+m*4] << 8) | (data[p1+5+m*4] << 16));
                node->status=(data[p1] & 2);
                node->p2=0;
                node->size = data[p1+1];
                node->p1=p1;
                return ((data[p1] & 1))+WORDCUT_DICT_WALK_OK; 
            }
        }
        return WORDCUT_DICT_WALK_FAIL;
    }
}
