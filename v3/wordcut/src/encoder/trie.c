#include "trie.h"
#include <assert.h>
#include <string.h>

#define MAXLEN  1024
#define MAX_POS 1024

static void
dump(StrCoder *pos_coder,TrieNode *node,unsigned char *buffer,int n)
{
  unsigned char ch;
  int i;
  assert (node!=NULL);
  if (n>=MAXLEN) WC_HALT("Word is too long.");
      
  if (node->terminator)
    {
      int i;
      wc_boolean foundpos=WC_FALSE;
      buffer[n]='\0';
      printf ("%s",buffer);

      for(i=0;i<node->pos->length;i++)
	{
	  printf ("\t%s",str_coder_str(pos_coder,node->pos->array[i]));
	  foundpos=WC_TRUE;
	}
      if (!foundpos) WC_HALT("NO Pos.");
      printf ("\n");

    }
  
  for(i=0;i<node->count;i++)
    {
      ch = node->list[i];
      if (node->child[ch]!=NULL)
	{
	  buffer[n]=ch;
	  dump(pos_coder,node->child[ch],buffer,n+1);
	}          
    }  
}


typedef struct bin_node_t {
  unsigned char type_term;
  unsigned char pos_len;
  unsigned char *pos_array;
  unsigned char len;
  unsigned char *key;
  unsigned int *child;
} BinNode;

typedef struct seq_node_t {
  unsigned char type_term;
  unsigned char pos_len;
  unsigned char *pos_array;
  unsigned char len;
  unsigned char *key;
  unsigned int child;
} SeqNode;

typedef struct pos_node_t {
  unsigned char type_term;
  unsigned char pos_len;
  unsigned char *pos_array;
} PosNode;

static void 
sortchar(unsigned char* t,size_t n)
{
  int i,j,k;
  for (i=0;i<n-1;i++)
    {
      k=i;
      for (j=i+1;j<n;j++)
	{
	  if (t[k]>t[j]) k=j;
	}
      if (k!=i)
	{
	  unsigned char c;
	  c=t[i];
	  t[i]=t[k];
	  t[k]=c;
	}
    }
}

static void
sort_key(TrieNode *node)
{
  int i;
  if (node->count>0)
    {
      sortchar(node->list,node->count);
      for(i=0;i<node->count;i++)
	{
	  sort_key(node->child[node->list[i]]);
	}
    }

}

static size_t 
seq_node_size(SeqNode* node)
{
  size_t size=6;
  size=size + node->pos_len + node->len;
  return size;
}

static size_t
bin_node_size(SeqNode* node)
{
  size_t size=3;
  size=size + node->pos_len + (node->len*4);
  return size;
}

static size_t
pos_node_size(PosNode* node)
{
  size_t size=2;
  size=size+node->pos_len;
  return size;
}

static void
pos_node_write(PosNode *node,FILE *fp)
{
  fwrite(&node->type_term,1,1,fp);
  fwrite(&node->pos_len,1,1,fp);
  if (node->pos_len>0)
    {
      fwrite(node->pos_array,1,node->pos_len,fp);
    }
}

static void
seq_node_write(SeqNode *node,FILE *fp)
{
  fwrite (&node->type_term,1,1,fp);
  fwrite (&node->pos_len,1,1,fp);

  if (node->pos_len>0)
    {
      printf ("!!! Write\n");
      fwrite(node->pos_array,1,node->pos_len,fp);
    }

  fwrite (&node->len,1,1,fp);

  
  if (node->len > 0)
    {
      fwrite(node->key,1,node->len,fp);
    }
  fwrite (&node->child,3,1,fp); 
}

static void
bin_node_write(BinNode *node,FILE *fp)
{
  fwrite(&node->type_term,1,1,fp);
  fwrite(&node->pos_len,1,1,fp);
  if (node->pos_len>0)
    {
      fwrite(node->pos_array,1,node->pos_len,fp);
    }
  fwrite(&node->len,1,1,fp);
  if (node->len>0)
    {
      int i;
      fwrite(node->key,1,node->len,fp);
      for(i=0;i<node->len;i++)
	{
	  fwrite(&node->child[i],3,1,fp);
	}
    }
}

dump_node(TrieNode *node)
{
  int i;
  printf("Addr=%ld\tCount=%d\tTerm=%c\n"
	 ,(unsigned int)node,node->count,node->terminator ? 'T' : 'F');
  /* list */
  for(i=0;i<node->count;i++)
    {
      printf ("%d,%c ",i,node->list[i]);
    }
  printf("\n");
}

static long
write_trie(TrieNode *node,FILE *fp,long start)
{
  int single_count=0;
  TrieNode *tmp=node;

  assert (node != NULL);

 
  while (node->count==1)
    {  
      dump_node(node);

      assert(node!=NULL);
      single_count++;
      node=node->child[node->list[0]];
      if(node->terminator) break;
    }
  dump_node(node);
  
  node=tmp;

  if (single_count>0)
    {

      SeqNode seq_node;
      int i;
      long buf,child_pos;
      printf ("!!! Single\n");
      printf ("!!! LINE=%d single_count=%d\n",__LINE__,single_count);    
      seq_node.type_term= (node->terminator ? 1 : 0);
      seq_node.type_term |= 0x30;
      /* add pos */
      if (node->pos!=NULL)
	{

	  if (node->pos->length > 0xFF) WC_HALT("Too many POS");
	  seq_node.pos_len=(unsigned char)node->pos->length;
	  seq_node.pos_array=node->pos->array;
	}
      else
	{
	  seq_node.pos_len=0;
	}
      if (single_count > 0xFF) WC_HALT("too long compressed node");
      seq_node.len=single_count;
      seq_node.key=WC_NEW_N(unsigned char,single_count);
      for(i=0;i<single_count;i++)
	{
	  seq_node.key[i]=node->list[0];
	  node=node->child[node->list[0]];
	}
      assert(node != NULL);
      child_pos=write_trie(node,fp,start); 

      if (child_pos > 0x00FFFFFF) WC_HALT("child pos is too much.");
      seq_node.child = child_pos;
      buf=ftell(fp);
      seq_node_write (&seq_node,fp);
      free(seq_node.key);
      return buf-start;
    }
  else 
    {
      long buf,child_pos;
      if (node->count>0)
	{
	  int i;
	  BinNode bin_node;
	  printf ("!!! BIN\n");
	  bin_node.type_term = (node->terminator ? 1 : 0);
	  bin_node.type_term |= 0x10;
	  /* add pos */
	  if (node->pos!=NULL)
	    {
	      if (node->pos->length > 0xFF) WC_HALT("Too many POS");
	      bin_node.pos_len=(unsigned char)node->pos->length;
	      bin_node.pos_array=node->pos->array;
	    }
	  else
	    {
	      bin_node.pos_len=0;
	    }
	  bin_node.len=node->count;
	  bin_node.child=WC_NEW_N(unsigned int,node->count);
	  bin_node.key=WC_NEW_N(unsigned char,node->count);
	  for(i=0;i<node->count;i++)
	    {
	      bin_node.key[i]=node->list[i];
	      child_pos=write_trie(node->child[node->list[i]],fp,start);
	      bin_node.child[i]=child_pos;
	      printf ("Bin child_pos=%d\n",child_pos);
	    }
	  buf=ftell(fp);
	 
	  bin_node_write(&bin_node,fp);
	  free(bin_node.child);
	  free(bin_node.key);
	  return buf-start;
	}
      else
	{
	  PosNode pos_node;
	  printf ("!!! Pos\n");
	  pos_node.type_term = (node->terminator ? 1 : 0);
	  pos_node.type_term |= 0x20;
	  if (node->pos!=NULL)
	    {
	      if (node->pos->length > 0xFF) WC_HALT("Too many POS");
	      pos_node.pos_len=(unsigned char)node->pos->length;
	      pos_node.pos_array=node->pos->array;
	    }
	  else
	    {
	      pos_node.pos_len=0;
	    }
	  buf=ftell(fp);
	  pos_node_write(&pos_node,fp);
	  return buf-start;
	}
    }
  return 0;
}

static void
write_pos(StrCoder* pos_coder,FILE *fp)
{
  int i;
  wc_byte len;

  fwrite(&pos_coder->length,sizeof(size_t),1,fp);
  for(i=0;i<pos_coder->length;i++)
    {
      printf ("POS=%s\n",pos_coder->array[i]);
      len=(unsigned char)strlen(pos_coder->array[i]);
      fwrite(&len,sizeof(wc_byte),1,fp);
      fwrite(pos_coder->array[i],sizeof(char),len,fp);
    }
}

WC_STATUS
trie_write(Trie* self,const char* output_filename)
{
  FILE *fp;  
  long ret,start,size_pos,root_pos;
  wc_uint32 size,root;
  if (self->pos_coder->length > 0x7F) return WC_RET_ERROR;
  fp=fopen(output_filename,"w");
  if (fp==NULL) return WC_RET_ERROR;
  write_pos(self->pos_coder,fp); 
  assert (self->root!=NULL);
  

  root_pos=ftell(fp);
  fwrite(&root,4,1,fp);

  size_pos=ftell(fp);
  printf ("Size pos=%ld\n",size_pos);
  size=0xFFFFFFFF;
  fwrite(&size,sizeof(wc_uint32),1,fp);

  
  start=ftell(fp);
  printf ("start =%ld\n",size_pos);
  ret=write_trie(self->root,fp,start);
  
  if (ret >= 0xFFFFFFFF) WC_HALT("Trie is too big");


  size=ftell(fp)-start;


  printf("Size=%lX\n",size);
  printf("size_pos=%ld\n",size_pos);
  fseek(fp,size_pos,SEEK_SET);
  fwrite(&size,4,1,fp);


  printf ("RET = %d\n",ret);
  root=ret;
  printf("root_pos=%ld\troot=%d\n",root_pos,root);
  fseek(fp,root_pos,SEEK_SET);
  fwrite(&root,4,1,fp);
  fclose(fp);
  return WC_RET_NORMAL;
}

void 
trie_dump(Trie* self)
{
  unsigned char buffer[MAXLEN];
  dump(self->pos_coder,self->root,buffer,0);
}

Trie* 
trie_new() 
{ 
  Trie* self=WC_NEW(Trie);
  trie_init(self);
  return self;
}

void 
trie_init(Trie* self)
{ 
  self->root=trie_node_new();
  self->pos_coder=str_coder_new(MAX_POS);
  self->ch_coder=char_coder_new();
}

static TrieNode*
add_char(TrieNode *node,char ch,wc_boolean terminator)
{
  TrieNode **next;
  assert(node != NULL);
  next=trie_node_child(node,ch);
  if (*next==NULL) 
    {
      node->list[node->count]=(unsigned char)ch;
      node->count++;
      *next=trie_node_new();
    }
  (*next)->terminator = (*next)->terminator || terminator;  
  return *next;
}

static WC_STATUS
add_word(Trie* self,Word *w)
{
  const char *p;
  const char *word; /* word surface */

  wc_boolean added=WC_FALSE;
  TrieNode *node=self->root;
  word=w->surface.data;
  char_coder_add_str(self->ch_coder,word);

  for(p=word;*p!='\0';p++) 
    {
      node=add_char(node,*p,*(p+1)=='\0');
      added=WC_TRUE;
    }

  if (added) 
    {
      size_t i;
      if (node->pos==NULL) 
	{
	  node->pos=pos_array_new();
	}
      for (i=0;i<w->pos_count;i++)
	{
	  WcString *a_pos=&w->pos[i];
	  const char* s_pos=a_pos->data;
	  char pos_id;
	  wc_boolean found;
	  str_coder_add(self->pos_coder,s_pos);
	  pos_id=str_coder_id(self->pos_coder,s_pos,&found);
	  if (!found) return WC_RET_ERROR;
	  /* add pos_id to pos array */
	  pos_array_add(node->pos,pos_id);
	}
    }
  return WC_RET_NORMAL;
}

WC_STATUS
trie_dict_add(Trie* self,
	      Dictionary *dict)
{
  size_t i;
  for(i=0;i<dict->length;i++)
    {
      add_word(self,&(dict->words[i]));
    }
  sort_key(self->root);
  return WC_RET_NORMAL;
}

void
trie_destroy(Trie *self)
{
  trie_node_destroy(self->root);
  str_coder_delete(self->pos_coder);
  char_coder_delete(self->ch_coder);
}

void
trie_delete(Trie *self)
{
  trie_destroy(self);
  free(self);
}
