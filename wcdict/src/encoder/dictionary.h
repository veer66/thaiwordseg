#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "word.h"

typedef struct dictionary_t 
{
  Word *words;
  size_t length,size,pos_size;
} Dictionary;

Dictionary *dictionary_new();
void dictionary_delete(Dictionary *self);
void dictionary_init(Dictionary *self);
void dictionary_destroy(Dictionary *self);
void dictionary_dump(Dictionary *self);
void dictionary_load(Dictionary *self,const char* dict_filename);
float dictionary_avg_pos_per_word(Dictionary *self);
#endif
