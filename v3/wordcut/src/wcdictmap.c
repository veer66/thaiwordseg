#include<wordcut/wcdict.h>

typedef struct 
{
  int start;
  WcDictIter iter;
} WordSeed;

typedef struct
{
  int start;
  WcDictIterPos pos;
} RawWord;

WcDictMap*
wc_dict_get_map(WcDict *self,const char *str,size_t len)
{
  return wc_dict_map_new(self,str,len);
}

void
wc_dict_map_init(WcDictMap *self,WcDict *dict,const char *str,size_t len)
{
  gint i,j;
  WordSeed *pool;
  gsize pool_size,pool_len;
  
  if (len==0) return;
  
  pool_len=0;
  pool_size=len;
  pool=g_new(WordSeed,pool_size);
  
  self->index=g_new(gint,len+1);
  self->assoc_tab=g_array_sized_new(FALSE,FALSE,sizeof(RawWord),len);

  for(i=0;i<len;i++)
    {      
      /* add new seed */
      if (pool_size==pool_len)
	{
	  pool_size=pool_size+pool_size;
	  g_renew(WordSeed,pool,pool_size);
	}

      pool[pool_len].start=i;
      wc_dict_root(dict,&(pool[pool_len].iter));
      pool_len++;
      
      /* transit */
      for(j=0;j<pool_len;j++)
	{	  
	  wc_dict_iter_transit(&(pool[j].iter),str[i]);
	}

      /* reject DEAD state seed */
      for(j=0;j<pool_len;)
	{
	  if (pool[j].iter.status==WC_DICT_ITER_DEAD)
	    {
	      pool_len--;
	      if(pool_len!=0 && pool_len != j)
		{
		  pool[j]=pool[pool_len];
		}
	    }
	  else 
	    {
	      j++;
	    }
	}

      self->index[i]=self->assoc_tab->len;
      /* select terminated seed */
      for(j=0;j<pool_len;j++)
	{
	  if (pool[j].iter.terminator)
	    {
	      RawWord raw_word;
#ifdef DEBUG
	      g_print ("*** ADD\n");
#endif
	      raw_word.start=pool[j].start;
	      wc_dict_iter_pos(&pool[j].iter,&raw_word.pos);
	      g_array_append_val(self->assoc_tab,raw_word);
	    }
	}      
    }
  self->index[i]=self->assoc_tab->len;
  self->len=len;
  free(pool);
}

void
wc_dict_map_destroy(WcDictMap *self)
{
  free(self->index);
  g_array_free(self->assoc_tab,FALSE);
}

void
wc_dict_map_delete(WcDictMap *self)
{
  wc_dict_map_destroy(self);
  free(self);
}

gint
wc_dict_map_assoc_len(WcDictMap *self,gint stop)
{
  if (stop>=self->len) return WC_DICT_MAP_NULL;

  if (self->index[stop+1]>=self->index[stop])
    {
      return self->index[stop+1]-self->index[stop];
    }  
  return WC_DICT_MAP_NULL;
}

gint 
wc_dict_map_assoc_at(WcDictMap *self,gint stop,gint offset)
{
  if (stop>=self->len || self->index[stop]+offset>=self->index[stop+1])
    return WC_DICT_MAP_NULL;
  return (g_array_index(self->assoc_tab,RawWord,self->index[stop]+offset)).start;
}

const WcDictIterPos*
wc_dict_map_assoc_pos_at(WcDictMap *self,gint stop,gint offset)
{
  if (stop>=self->len || self->index[stop]+offset>=self->index[stop+1])
    return NULL;
  return &(g_array_index(self->assoc_tab,RawWord, 
			 self->index[stop]+offset).pos);

}

WcDictMap *
wc_dict_map_new(WcDict *dict,const gchar *str,gsize len)
{
  WcDictMap *self=g_new(WcDictMap,1);
  wc_dict_map_init(self,dict,str,len);
  return self;
}
