#include<stdio.h>
#include"dictionary.h"
#include"trie.h"

static void 
test_dict(const char *filename)
{
  Dictionary dict;
  dictionary_init(&dict);
  dictionary_load(&dict,filename);
  /* dictionary_dump(&dict); */
  printf ("%f\n",dictionary_avg_pos_per_word(&dict)); 
  dictionary_destroy(&dict);
}

static void 
test_trie(const char *dictfile)
{
  Dictionary *dict;
  Trie *trie;
  dict=dictionary_new();
  dictionary_load(dict,dictfile);
  trie=trie_new();
  trie_dict_add(trie,dict);
  trie_dump(trie);
  trie_delete(trie);
  dictionary_delete(dict); 
}

static void
test_write_trie(const char *dict_file)
{
  Dictionary *dict;
  Trie *trie;
  dict=dictionary_new();
  dictionary_load(dict,dict_file);
  trie=trie_new();
  trie_dict_add(trie,dict);
  trie_dump(trie);
  if (trie_write(trie,"encodedtrie.etd")!=WC_RET_NORMAL)
    {
      WC_HALT("can't write trie");
    }

  trie_delete(trie);
  dictionary_delete(dict);
}

static void
encode(const char *dict_file,const char *target)
{
  Dictionary *dict;
  Trie *trie;
  dict=dictionary_new();
  dictionary_load(dict,dict_file);
  trie=trie_new();
  trie_dict_add(trie,dict);
  trie_dump(trie);
  if (trie_write(trie,target)!=WC_RET_NORMAL)
    {
      WC_HALT("can't write trie");
    }

  trie_delete(trie);
  dictionary_delete(dict);
}


int 
main (int argc ,char** argv) 
{

  if (argc < 3)
    {
      fprintf (stderr,"Usage: wcencoder <plain text dictionary> <encoded dictionary\n");
      exit(1);
    }
  encode(argv[1],argv[2]);
  return 0;
}
