#include<wordcut/wcdictmap.h>
/* #define DEBUG */

typedef struct active_list_t
{
  WcDictMapTok *tok;
  size_t size,len;
} ActiveList;

static void
active_list_init(ActiveList *self,size_t size)
{
  self->size=size;
  self->len=0;
  self->tok=WC_NEW_N(WcDictMapTok,size);
}

static void
active_list_destroy(ActiveList *self)
{
  free(self->tok);
}

static void
active_list_insert(ActiveList *self,int start,WcDict* dict)
{
  assert(self->size>=self->len);
  if (self->size==self->len)
    {
      /* resize */
      self->size = self->size + self->size;
      self->tok = WC_RENEW(WcDictMapTok,self->tok,self->size);
    }
  self->tok[self->len].start=start;
  wc_dict_root(dict,&self->tok[self->len].iter);

  self->len++;  
}

static void
active_list_update(ActiveList *self,char ch)
{
  int i;
  /* transit */
  for(i=0;i<self->len;i++)
    {
      wc_dict_iter_transit(&(self->tok[i].iter),ch);
    }

  /* delete dead node(s) */
  for(i=0;i<self->len;) 
   {
      if (self->tok[i].iter.status==WC_DICT_ITER_DEAD)
	{
	  self->len--;
	  if(self->len!=0 && self->len != i)
	    self->tok[i]=self->tok[self->len];
	}
      else 
	{
	  i++;
	}
    }
}

WcDictMap*
wc_dict_get_map(WcDict *self,const char *str,size_t len)
{
  return wc_dict_map_new(self,str,len);
}


static void
insert_map_tok(WcDictMap *self,ActiveList* list,int i)
{

#ifdef DEBUG
  printf ("Len=%d\tSize=%d\tstart=%d\n",
	  self->len,self->size,list->tok[i].start);
  
#endif
  assert (self->len<=self->size);
  /* resize if full */
  if (self->len==self->size)
    {
      self->size += self->size;
      self->tok=WC_RENEW(WcDictMapTokPos,self->tok,self->size);
    }

  /* config tok+position */
  self->tok[self->len].start=list->tok[i].start;
  wc_dict_iter_pos(&list->tok[i].iter, &self->tok[self->len].pos );
  self->len++;
}

static void
update_tab(WcDictMap* self,WcDict* dict,ActiveList* active_list)
{
  int i;
  for(i=0;i<active_list->len;i++)
    {
      if(active_list->tok[i].iter.terminator)
	insert_map_tok(self,active_list,i);
    }
}

void
wc_dict_map_init(WcDictMap *self,WcDict *dict,const char *str,size_t len)
{
  
  ActiveList active_list;
  int i;

#ifdef DEBUG
  printf ("DICT MAP STRLEN=%d\n",len);
#endif
  
  if (len==0) return;  
  self->len=0;

  self->size=len+len;
  self->index=WC_NEW_N(int,len+1);
  
  self->tok=WC_NEW_N(WcDictMapTokPos,self->size);
  
  active_list_init(&active_list,len);
  
  for(i=0;i<len;i++)
    {            
      self->index[i]=self->len;      
      active_list_insert(&active_list,i,dict);
      active_list_update(&active_list,str[i]);
      update_tab(self,dict,&active_list);      
    }
  
  self->index[len]=self->len;
  self->strlen=len;

  
  active_list_destroy(&active_list);
}

void
wc_dict_map_destroy(WcDictMap *self)
{
  free(self->tok);
  free(self->index);
}

void
wc_dict_map_delete(WcDictMap *self)
{
  wc_dict_map_destroy(self);
  free(self);
}

int
wc_dict_map_assoc_len(WcDictMap *self,int stop)
{
  if (stop>=self->strlen) return WC_DICT_MAP_NULL;
#ifdef DEBUG
  printf ("ASSOC_LEN\tstop=%d\tindex[stop+1]=%d\tindex[stop]=%d\n",
	  stop,self->index[stop+1],self->index[stop]);
#endif
  if (self->index[stop+1]>=self->index[stop])
    {
      return self->index[stop+1]-self->index[stop];
    }  
  return WC_DICT_MAP_NULL;
}


/* return start position of word */
int 
wc_dict_map_assoc_at(WcDictMap *self,int stop,int offset)
{
  if (stop>=self->strlen || self->index[stop]+offset>=self->index[stop+1])
    return WC_DICT_MAP_NULL;
  return self->tok[((self->index[stop])+offset)].start;
}

const WcDictIterPos*
wc_dict_map_assoc_pos_at(WcDictMap *self,int stop,int offset)
{
  if (stop>=self->strlen || self->index[stop]+offset>=self->index[stop+1])
    return NULL;
  return &self->tok[((self->index[stop])+offset)].pos;

}

WcDictMap *
wc_dict_map_new(WcDict *dict,const char *str,size_t len)
{
  WcDictMap *self=WC_NEW(WcDictMap);
  wc_dict_map_init(self,dict,str,len);
  return self;
}
