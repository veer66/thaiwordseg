#ifndef __TRIENODE_H__
#define __TRIENODE_H__

#include<wordcut/wccommon.h>
#include"posarray.h"
#define MAPSIZE	0x100

typedef struct trienode_t {
  struct trienode_t *child[MAPSIZE];
  unsigned char list[MAPSIZE];
  size_t count;
  wc_boolean terminator;
  PosArray *pos;
} TrieNode;

void trie_node_init(TrieNode* self);
TrieNode* trie_node_new();
void  trie_node_destroy(TrieNode *self);
void trie_node_delete(TrieNode *self);
TrieNode** trie_node_child(TrieNode *self,char ch);

#endif
