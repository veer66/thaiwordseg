#include "trienode.h"

void 
trie_node_init(TrieNode* self)
{
  self->terminator=WC_FALSE;
  memset(self->child,0,sizeof(self->child));
  self->pos=NULL;
  self->count=0;
}


TrieNode**
trie_node_child(TrieNode *self,char ch)
{
  return &(self->child[(unsigned char)ch]);
}

TrieNode*
trie_node_new()
{
  TrieNode *self=WC_NEW(TrieNode);
  trie_node_init(self);
  return self;
}

void 
trie_node_destroy(TrieNode *self)
{
  size_t i;
  if (self->pos!=NULL) 
    {
      free(self->pos);
    }
  for(i=0;i<MAPSIZE;i++) 
    {
      if (self->child[i]!=NULL) 
	{
	  trie_node_delete(self->child[i]);
	}
    }
}

void
trie_node_delete(TrieNode *self)
{
  trie_node_destroy(self);
  if (self->pos!=NULL)
    {
      pos_array_delete(self->pos);
    }
  free(self);
}
