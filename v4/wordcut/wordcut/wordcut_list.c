
/* 
 * Copyright (C) 2004 Vee Satayamas
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
 */


#include<wordcut/xmalloc.h>
#include<wordcut/wordcut_list.h>

int
wordcut_list_init(WordcutList *self, size_t max_size)
{
    if (max_size <= 0) 
    {
        self->content = NULL;
        return 1;
    }
    self->max_size = max_size;
    self->size = 0;
    self->content = xmalloc(sizeof(void *) * self->max_size);  
    return 0;
}

int 
wordcut_list_append(WordcutList *self, void* data)
{
    if(self->size == self->max_size) 
    {
        self->max_size = self->max_size * 2;
        self->content = xrealloc(self->content, sizeof(void *) * self->max_size);
    }
    self->content[self->size] = data;
    ++self->size;
    return 0;
}

int 
wordcut_list_destroy(WordcutList *self)
{
    free(self->content);
    return 0;
}
/*
int main ()
{
    WordcutList lst;
    int i;
    wordcut_list_init(&lst,10);    
    for(i=0;i<20;i++) {    
        wordcut_list_append(&lst, (void*)i);
    }
    printf ("%d\n", wordcut_list_get(&lst,10) );
    wordcut_list_destroy(&lst);    
    return 0;    
} */
