#ifndef __TRIE_H__
#define __TRIE_H__
#include "trienode.h"
#include "dictionary.h"
#include "strcoder.h"
#include "charcoder.h"

typedef struct trie_t 
{
  TrieNode *root;
  StrCoder *pos_coder;
  CharCoder *ch_coder;
} Trie;

Trie* trie_new();
void trie_init(Trie* self);
void trie_destroy(Trie *self);
void trie_delete(Trie *self);
WC_STATUS trie_dict_add(Trie* self, Dictionary *dict);
void trie_dump(Trie* self);
WC_STATUS trie_write(Trie* self,const char* output_filename);

#endif
