#include "dictionary.h"
#include <string.h>
#define DEFAULT_DICTIONARY_SIZE   30000  /* words */
#define DEFAULT_POS_PER_WORD      10

Dictionary*
dictionary_new() 
{
  Dictionary *self=WC_NEW(Dictionary);
  dictionary_init(self);
  return self;
}

void
dictionary_init(Dictionary *self) 
{
  self->length=0;
  self->pos_size=DEFAULT_POS_PER_WORD;
  self->size=DEFAULT_DICTIONARY_SIZE;
  self->words=WC_NEW_N(Word,DEFAULT_DICTIONARY_SIZE);  
}

void
dictionary_destroy(Dictionary *self)
{
  int i,j;
  for(i=0;i<self->length;i++) 
    {
      free(self->words[i].surface.data);
      for(j=0;j<self->words[i].pos_count;j++) 
	{
	  free(self->words[i].pos[j].data);
	}
    }
  free(self->words);
}

void 
dictionary_delete(Dictionary *self) 
{
  assert(self!=NULL);
  free(self);
}

void
dictionary_load(Dictionary *self,const char* dict_filename)
{
#define MAX_BUFFER 4096
  FILE *fp;
  char buffer[MAX_BUFFER+1];
  char *word;
  
  if ((fp=fopen(dict_filename,"r"))!=NULL) 
    {
      while(fgets(buffer,MAX_BUFFER,fp)!=NULL) 
	{
	  word=strtok(buffer," \n\t\r");
	  if (word!=NULL) 
	    {
	      Word *w;
	      char *pos;
	      if ((self->length+1) >= self->size) 
		WC_HALT("Dictionary is too big");
	      /* add word */

	      w=&self->words[self->length];
	      wc_string_init_with_str(&w->surface,word);
	      w->pos=WC_NEW_N(WcString,self->pos_size);
	      /* get pos */
	      w->pos_count=0;
	      while ((pos=strtok(NULL," \t\r\n"))!=NULL)
		{
		  assert (pos!=NULL);
		  if (w->pos_count+1 >= self->pos_size) 
		    WC_HALT("Too many POS for a word");
		  wc_string_init_with_str(&w->pos[w->pos_count],pos);		       
		  w->pos_count++;		  
		}
	      self->length++;
	    }
	}
    }
  else
    {
      WC_HALT("Can't open dictionary file.");
    }
  fclose(fp);
}


void
dictionary_dump(Dictionary *self)
{
  size_t a_word_len = self->length;
  size_t i;
  self->words=self->words;
  for (i=0;i<a_word_len;i++) 
    {
      Word *word=&(self->words[i]);
      int j;
      /* print surface */
      printf ("%s",word->surface.data);

      /* travel on pos array and print them */
      for(j=0;j<word->pos_count;j++) 
	{
	  printf ("\t%s",word->pos[j].data);
	}
      printf ("\n");
    }

}

float
dictionary_avg_pos_per_word(Dictionary *self)
{
  int t=0;
  size_t i;
  for (i=0;i<self->length;i++)
    {
      t=t+self->words[i].pos_count;
    }
  return (float)t/(float)self->length;
}
