#include<wordcut/wcdict.h>

#define WC_DICT_MAP_NULL -1


typedef struct wc_dict_map_tok_t
{
  int start;
  WcDictIter iter;
}WcDictMapTok;

typedef struct wc_dict_map_tok_pos_t
{
  int start;
  WcDictIter iter;
  WcDictIterPos pos;
}WcDictMapTokPos;

typedef struct wc_dict_map_t
{
  int* index;
  WcDictMapTokPos *tok;
  size_t strlen;
  size_t len;
  size_t size;
} WcDictMap;


void wc_dict_map_init(WcDictMap *self,WcDict *dict,const char *str,size_t len);
void wc_dict_map_destroy(WcDictMap *self);
void wc_dict_map_delete(WcDictMap *self);
int wc_dict_map_assoc_len(WcDictMap *self,int stop);
int wc_dict_map_assoc_at(WcDictMap *self,int stop,int offset);
WcDictMap* wc_dict_map_new(WcDict *dict,const char *str,size_t len);
WcDictMap* wc_dict_get_map(WcDict *self,const char *str,size_t len);
const WcDictIterPos* wc_dict_map_assoc_pos_at(WcDictMap *self,int stop,int offset);



typedef struct active_list_t
{
  WcDictMapTok *tok;
  size_t size,len;
} ActiveList;

static void
active_list_init(ActiveList *self,size_t size)
{
  self->size=size;
  size->len=0;
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
  wc_dict_iter_root(dict,&(self->tok[self->len].iter));
  self->len++;  
}

static void
active_list_update(ActiveList *self,char ch)
{
  int i;

  /* transit */
  for(i=0;i<self->len;i++)
    wc_dict_iter_transit(&(self->tok[i].iter),ch);

  /* delete dead node(s) */
  for(i=0;i<self->len;) 
   {
      if (self->iter_list[i].status==WC_DICT_ITER_DEAD)
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
  assert (self->len<=self->size);
  if (self->len==self->size)
    {
      self->size += self->size;
      self->tok=WC_RENEW(WcDictMapTokPos,self->tok,self->size);
    }
  self->tok[self->len].start=list[i].start;
  self->tok[self->len].iter=list[i].iter;
  /* TODO: Add Pos */
        self->tok[self->len].pos

  
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
  if (len==0) return;  
  active_list_init(&active_list,len);
  for(i=0;i<len;i++)
    {      
      active_list_insert(&active_list,i,dict);
      active_list_update(&active_list,str[i]);
      self->index[i]=self->len;
    }
  self->index[i]=self->len;
  self->strlen=len;
  active_list_destroy(&active_list);
}

void
wc_dict_map_destroy(WcDictMap *self)
{
  free(self->index);
  /* TODO */
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
