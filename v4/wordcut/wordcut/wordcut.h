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


#ifndef __WORDCUT_H__
#define __WORDCUT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <wordcut/wordcut_dict.h>

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

int wordcut_init(Wordcut *self,const char *dict_filename);
void wordcut_cut(Wordcut *self,const char *str,WordcutResult *result);
void wordcut_close(Wordcut *self);
void wordcut_result_close(WordcutResult *self);

#ifdef __cplusplus
}
#endif

#endif
