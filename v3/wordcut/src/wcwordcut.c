#include<wordcut/wcwordcut.h>
#include <assert.h>

static void
join_front(WcWordcutResult* result,
	   int stop,
	   int front_start)
{
  result->tab[stop].start=front_start;
  result->tab[stop].type=WC_WORD_TYPE_JOIN;
  result->tab[stop].pos=NULL;
  result->reverse[front_start]=stop;
}

static void
join_back(WcWordcutResult* result,
	  int start,
	  int back_stop)
{
  result->tab[back_stop].start=start;
  result->tab[back_stop].type=WC_WORD_TYPE_JOIN;
  result->tab[back_stop].pos=NULL;
  result->reverse[start]=back_stop;
}

static void
join_unk(WcWordcutResult *result)
{
  int i,c;
  if (result->len<=1) return ;
  for(i=0;i<result->n;i++)
    {
      int stop=result->index[i];
      int start=result->tab[stop].start;
      int len=stop-start+1;
      int front_len,back_len,front_start,front_stop,back_start,back_stop;
      

      if (len==1 && result->str[start]!='.')
	{
	  if (start==0)
	    {
	      back_stop=result->reverse[stop+1];
	      join_back(result,start,back_stop); 
	    }
	  else if (stop==result->len-1)
	    {
	      front_stop=start-1;
	      front_start=result->tab[front_stop].start;
	      join_front(result,stop,front_start);
	    }
	  else
	    {	      
	      front_stop=start-1;
	      front_start=result->tab[front_stop].start;
	      front_len=front_stop-front_start+1;


	      back_start=stop+1;
	      back_stop=result->reverse[back_start];
	      back_len=back_stop-back_start+1;

	      if(back_len<front_len)
		{
		  join_back(result,start,back_stop);
		}
	      else
		{
		  join_front(result,stop,front_start);
		}	      
	    }
	}
    }
  i=result->len-1;
  c=0;
  while(i>=0)
    {
      int start;
      start=result->tab[i].start;
#ifdef DEBUG3
      printf ("-->start=%d\ti=%d\n",start,i);
#endif

      result->index[c++]=i;
      i=start-1;
    }
  result->n=c;
}

static void 
dup_pos(WcWordcutResult *result)
{
  int i,start;
  const WcDictIterPos *p2;
  for(i=result->len-1;i>=0;i=start-1)
    {
      start=result->tab[i].start;
      if ((p2=result->tab[i].pos)!=NULL)
	{
	  WcDictIterPos *pos=WC_NEW(WcDictIterPos);      
	  memcpy(pos,p2,sizeof(WcDictIterPos));
	  result->tab[i].pos=pos;
	}
    }
}


static int 
element_cmp(WcWordcutTableElement *a,
	    WcWordcutTableElement *b)
{
  if(a->brk_c == b->brk_c)
    {
      if (a->unk_ch_c == b->unk_ch_c)
	{
	  if (a->wu_c == b->wu_c)
	    {
	      if(a->dict_c == b->dict_c)
		{
		  return 0;
		}
	      else
		{
		  return (( a->dict_c < b->dict_c) ? -1 : 1);
		}
	    }
	  else
	    {
	      return (( a->wu_c < b->wu_c ) ? -1 : 1);
	    }
	}
      else
	{
	  return ((a->unk_ch_c < b->unk_ch_c) ? -1 : 1);
	}
    }
  else
    {
      return ((a->brk_c < b->brk_c) ? -1 : 1);
    }
}




/*
 * copy_ele function perform partial copy 
 * just for compare best path
 */
static void
set_element(WcWordcutTableElement* tab,WcWordcutTableElement* target,
	 int start,int stop,WcWordType wt,WcWordunitMap *wu_map)
{
  if (start >= 0)
    {
      target->brk_c=tab[start].brk_c + 
	(wc_wordunit_map_break(wu_map,start,stop) ? 1 : 0);
      target->unk_ch_c=tab[start].unk_ch_c +
	(wt==WC_WORD_TYPE_UNK ? 1 : 0);
      target->wu_c=tab[start].wu_c +
	(wt==WC_WORD_TYPE_WORDUNIT ? 1 : 0);
      target->dict_c=tab[start].dict_c +
	(wt==WC_WORD_TYPE_DICT ? 1 : 0);  
    }
  else
    {
      target->brk_c=(wc_wordunit_map_break(wu_map,start,stop) ? 1 : 0);
      target->unk_ch_c=(wt==WC_WORD_TYPE_UNK ? 1 : 0);
      target->wu_c=(wt==WC_WORD_TYPE_WORDUNIT ? 1 : 0);
      target->dict_c=(wt==WC_WORD_TYPE_DICT ? 1 : 0);  
    }
}


static void
dump_element(WcWordcutTableElement *tab)
{
  printf ("start=%d break=%d unk=%d wordunit=%d dict=%d\n",
	  tab->start,tab->brk_c,tab->unk_ch_c,tab->wu_c,tab->dict_c);
}


static void 
select_path(WcDictMap *dict_map,WcWordunitMap *wu_map,
	    char ch,WcWordcutTableElement* tab,int n,wc_boolean debug)
{
  WcWordcutTableElement selected,canidate;
  wc_boolean first=WC_TRUE;
  int start,i,len;
  
  /* from wordunit */
  start=wc_wordunit_map_assoc(wu_map,n);
  if(start!=WC_WORDUNIT_NULL)
    {
      set_element(tab,&canidate,start-1,n,
		  WC_WORD_TYPE_WORDUNIT,wu_map);
      if (first || element_cmp(&selected,&canidate)>0)
	{
	  first=WC_FALSE;
	  selected=canidate;
	  selected.pos=NULL;
	  selected.type=WC_WORD_TYPE_WORDUNIT;
	  selected.start=start;
	  
	  if (debug) 
	    {
	      /* debug select path */
	      printf ("Select\tCanidate\n");
	      dump_element(&selected);
	      dump_element(&canidate);
	      printf ("%s: n=%d\tType = %c\n",__FILE__,n,selected.type);
	    }
	}
    }
  

  /* from dict */
  len=wc_dict_map_assoc_len(dict_map,n);
  for(i=0;i<len;i++)
    {
      start=wc_dict_map_assoc_at(dict_map,n,i);
      assert(start!=WC_DICT_MAP_NULL);
      set_element(tab,&canidate,start-1,n,
		  WC_WORD_TYPE_DICT,wu_map);
      if(first || element_cmp(&selected,&canidate)>0)
	{
	  first=WC_FALSE;
	  selected=canidate;
	  selected.type=WC_WORD_TYPE_DICT;
 	  selected.pos=wc_dict_map_assoc_pos_at(dict_map,n,i);

	  if(debug)
	    {
	      printf ("Select\tCanidate\n");
	      dump_element(&selected);
	      dump_element(&canidate);
	      printf ("%s: n=%d\tType=%c\tPos len = %d\n",
		      __FILE__,n,selected.type,selected.pos->len);
	    }

	  //assert(selected.pos!=NULL);
	  selected.start=start;
	}
    }

  /* unknown */
  set_element(tab,&canidate,n-1,n,WC_WORD_TYPE_UNK,wu_map);
  if(first || element_cmp(&selected,&canidate)>0)
    {
      first=WC_FALSE;
      selected=canidate;
      selected.type=WC_WORD_TYPE_UNK;
      selected.pos=NULL;
      selected.start=n;

      if(debug)
	{
	  printf ("Select\tCanidate\n");
	  dump_element(&selected);
	  dump_element(&canidate);
	  printf ("%s: n=%d\tType = %c\n",__FILE__,n,selected.type);
	}
    }
  tab[n]=selected;
  if(debug)
    {
      printf ("N=%d\tTab[n] type=%c ",n,tab[n].type);
      dump_element(&tab[n]);
    }
}


#ifdef DEBUG
static void
dump_tab(WcWordcutTableElement *tab,gsize len)
{
  int i;
  for(i=0;i<len;i++)
    {
      printf ("i=%d start=%d break=%d unk=%d wordunit=%d dict=%d\n",
	      i,tab[i].start,tab[i].brk_c,tab[i].unk_ch_c,
	      tab[i].wu_c,tab[i].dict_c);
	      
    }
}
#endif

#ifdef DEBUG
static WC_STATUS
tab2str(WcWordcutTableElement *tab,
	const char *src,size_t len,
	char *out,size_t outlen,const char* delimiter,size_t dimlen)
{
  int i,start,j;
  int *index;
  size_t n;
  wc_boolean first=TRUE;
  
  index=WC_NEW_N(gint,len);
  i=len-1;
  n=0;

  printf ("!!!1\n");

  while (i>=0)
    {
      start=tab[i].start;
      index[n]=i;    
      n++;
      i=start-1;
    }

  printf ("!!!2\n");

  if (n<=0) return WC_RET_ERROR;
  if (dimlen*(n-1)+len+1>=outlen) return WC_RET_ERROR;

  printf ("!!! n=%d\n",n);

  for(i=n-1;i>=0;i--)
    {

      printf ("!!!3 start=%d stop=%d\n",tab[index[i]].start,index[i]);

      if (first)
	{
	  first=FALSE;
	}
      else
	{
	  for(j=0;j<dimlen;j++)
	    {
	      *out++ = delimiter[j];
	    }
	}
      
      for(j=tab[index[i]].start;j<=index[i];j++)
	{
	  *out++ = src[j];
	}
    }
  *out='\0';  

  free(index);
  return WC_RET_NORMAL;
}
#endif      

#ifdef DEBUG
static void
dump_result(WcWordcutResult *self)
{
  int i;
  printf ("Len=%d Str=%s\n",self->len,self->str);
  printf ("Tab start : ");
  for(i=0;i<self->len;i++)
    {
      printf ("%d ",self->tab[i].start);
    }
  printf ("\n");
  printf ("n=%d\n",self->n);
  printf ("Index = ");
  for(i=0;i<self->n;i++)
    {
      printf ("%d ",self->index[i]);
    }
  printf ("\n");
}
#endif
int
wc_wordcut_result_str(WcWordcutResult* self , char *out , size_t out_size   ,
		      const char *delimiter,size_t del_len)
{
  int i,j,l=0;
  wc_boolean first=WC_TRUE;
  if (self->n*del_len+1>=out_size || self->n<=0)
    return -1;
  for(i=self->n-1;i>=0;i--)
    {
      if(!first)
	{
	  for(j=0;j<del_len;j++)
	    {
	      *out++ = delimiter[j];
	      l++;
	    }
	}
      else
	{
	  first=WC_FALSE;
	}
#ifdef DEBUG
      printf("Stop=%d\n",self->index[i]);
      printf ("Start=%d\n",self->tab[self->index[i]].start);
#endif      
      for(j= self->tab[self->index[i]].start ; j<=self->index[i] ; j++)	    
	{
	  *out++ = self->str[j];
	  l++;
	}
    }
  *out='\0';
  return l;
}
int
wc_wordcut_result_str_tag(WcWordcutResult* self , char *out , size_t out_size   ,
		      const char *delimiter,size_t del_len)
{
  int i,k,j,l=0;
  const char *pch;
  wc_boolean first=WC_TRUE;
  const WcDictIterPos *pos;
  wc_byte id;
  const char *posstr;

  if (self->n*del_len+1>=out_size || self->n<=0)
    return -1;
  for(i=self->n-1;i>=0;i--)
    {
      if(!first)
	{
	  for(j=0;j<del_len;j++)
	    {
	      *out++ = delimiter[j];
	      l++;
	    }
	}
      else
	{
	  first=WC_FALSE;
	}
#ifdef DEBUG
      printf("Stop=%d\n",self->index[i]);
      printf ("Start=%d\n",self->tab[self->index[i]].start);
#endif      
      for(j= self->tab[self->index[i]].start ; j<=self->index[i] ; j++)	    
	{
	  *out++ = self->str[j];
	  l++;
	}
      
      pos=wc_wordcut_result_pos_at(self,i);
      if (pos!=NULL)
	{
	  for(k=0;k<pos->len;k++)
	    {
	      size_t pos_len=strlen(posstr);
	      wc_dict_iter_pos_posid(pos,k,&id);
	      posstr=wc_dict_iter_pos_posid2str(pos,id);
	      if (l+pos_len+2>=out_size) return -1;
	      *out++='<';
	      for(pch=posstr;*pch!='\0';pch++)
		{
		  *out++ = *pch;
		}	
	      *out++='>';
	      l=l+2+pos_len;
	    }
	}
    }
  *out='\0';
  return l;
}


void 
wc_wordcut_set_debug_select_path(WcWordcut* self)
{
  self->debug_select_path=WC_TRUE;
}

void
wc_wordcut_cut(WcWordcut *self,const char* str,int len,WcWordcutResult *result)
{
  int i,start,c;
  WcDictMap dict_map;
  WcWordunitMap wu_map;

  wc_wordunit_map_init(&wu_map,str,len);
  wc_dict_map_init(&dict_map,self->dict,str,len);
  
  result->tab=WC_NEW_N(WcWordcutTableElement,len);

  for(i=0;i<len;i++)
    {
      select_path(&dict_map,&wu_map,str[i],result->tab,i,self->debug_select_path);
    }

  result->len=len;

  result->str=WC_NEW_STR(len);
  strncpy(result->str,str,len);
  result->str[len]='\0';
  

  i=result->len-1;
  c=0;
 
  result->index   = WC_NEW_N(int,len);
  result->reverse = WC_NEW_N(int,len);
  
  while(i>=0)
    {
      start=result->tab[i].start;
      result->index[c]=i;
      result->reverse[start]=i;
      i=start-1;
      c++;
    }
  result->n=c;
  join_unk(result); 
  dup_pos(result);
  wc_dict_map_destroy(&dict_map);
  wc_wordunit_map_destroy(&wu_map);
}      

void
wc_wordcut_result_destroy(WcWordcutResult *self)
{
  int i,start;
  const WcDictIterPos *p2;
  for(i=self->len-1;i>=0;i=start-1)
    {
      start=self->tab[i].start;
      if ((p2=self->tab[i].pos)!=NULL)
	{
	  free((WcDictIterPos*)p2);
	}
    }
  
  free(self->tab);
  free(self->str);
  free(self->index);
  free(self->reverse);
}


WcWordcut*
wc_wordcut_new_with_dict(WcDict *dict)
{
  WcWordcut *self;
  self=WC_NEW(WcWordcut);
  wc_wordcut_init_with_dict(self,dict);
  return self;
}

void 
wc_wordcut_init_with_dict(WcWordcut *self,WcDict *dict)
{
  self->debug_select_path=WC_FALSE;
  self->dict=dict;
  self->ext_dict=WC_TRUE;
}


void 
wc_wordcut_init(WcWordcut *self,WC_STATUS *error)
{
  self->ext_dict=WC_FALSE;
  self->debug_select_path=WC_FALSE;
  self->dict=wc_dict_new();
  if (wc_dict_load(self->dict,WC_SHARE_DATA_PATH "dict.etd")==WC_RET_NORMAL)
    *error=WC_RET_NORMAL;
  else
    *error=WC_RET_ERROR;

}

void 
wc_wordcut_init_with_dictfile(WcWordcut *self,const char *dict_filename,WC_STATUS *error)
{
  self->ext_dict=WC_FALSE;
  self->dict=wc_dict_new();
  self->debug_select_path=WC_FALSE;
  if (wc_dict_load(self->dict,dict_filename)==WC_RET_NORMAL)
    *error=WC_RET_NORMAL;
  else
    *error=WC_RET_ERROR;

}

WcWordcut*
wc_wordcut_new()
{
  WcWordcut *self;
  WC_STATUS error;
  self=WC_NEW(WcWordcut);
  wc_wordcut_init(self,&error);
  if (error==WC_RET_ERROR) return NULL;
  return self;
}

WcWordcut*
wc_wordcut_new_with_dictfile(const char *dict_filename)
{
  WcWordcut *self;
  WC_STATUS error;
  self=WC_NEW(WcWordcut);
  wc_wordcut_init_with_dictfile(self,dict_filename,&error);
  if (error==WC_RET_ERROR) return NULL;
  return self;
}

void 
wc_wordcut_destroy(WcWordcut *self)
{
  if (!self->ext_dict)
    {
      wc_dict_delete(self->dict);
    }
}

void
wc_wordcut_delete(WcWordcut *self)
{
  wc_wordcut_destroy(self);
  free(self);
}

typedef struct
{
  char *out;
  size_t size;
  WC_STATUS status;
  const char *delimiter;
  size_t del_len;
  WcWordcut *wordcut;
} CallbackData;

static void 
thai_callback(const char *str,size_t len,void *data)
{
  int l;
  CallbackData *cb_data=(CallbackData *)data;
  WcWordcutResult result;
  if (cb_data->status==WC_RET_ERROR) return;
  wc_wordcut_cut(cb_data->wordcut,str , len , &result);
#ifdef DEBUG
  dump_result(&result);
#endif 
  l=wc_wordcut_result_str(&result,cb_data->out,cb_data->size,
			  cb_data->delimiter,cb_data->del_len);
  if(l==-1) 
    {
      cb_data->status=WC_RET_ERROR;
      return;
    }
  cb_data->out=cb_data->out+l;
  cb_data->size -= l;
  wc_wordcut_result_destroy(&result);
}


static void 
thai_callback_tag(const char *str,size_t len,void *data)
{
  int l;
  CallbackData *cb_data=(CallbackData *)data;
  WcWordcutResult result;
  if (cb_data->status==WC_RET_ERROR) return;
  wc_wordcut_cut(cb_data->wordcut,str , len , &result);
#ifdef DEBUG
  dump_result(&result);
#endif 
  l=wc_wordcut_result_str_tag(&result,cb_data->out,cb_data->size,
			  cb_data->delimiter,cb_data->del_len);
  if(l==-1) 
    {
      cb_data->status=WC_RET_ERROR;
      return;
    }
  cb_data->out=cb_data->out+l;
  cb_data->size -= l;
  wc_wordcut_result_destroy(&result);
}


static void
default_callback(const char *str,size_t len,void *data)
{
   CallbackData *cb_data=(CallbackData *)data;
   int j;
   if (cb_data->status==WC_RET_ERROR) return;
   if (len+1>=cb_data->size)
     {
       cb_data->status=WC_RET_ERROR;
       return;
     }
   for(j=0;j<len;j++)
     {
       *(cb_data->out) = str[j];
       cb_data->out=cb_data->out+1;
     }
   cb_data->size -= len;
}

WC_STATUS
wc_wordcut_cutline(WcWordcut *self,const char* str,
		   char *out,size_t out_size,
		   const char *delimiter,
		   size_t del_len)
{
  CallbackData data;
  data.size=out_size;
  data.out=out;
  data.delimiter=delimiter;
  data.del_len=del_len;
  data.status=WC_RET_NORMAL;
  data.wordcut=self;
  wc_split(str,thai_callback,&data,default_callback,&data);
  *data.out='\0';
  return data.status;
}


WC_STATUS
wc_wordcut_cutline_tag(WcWordcut *self,const char* str,
		   char *out,size_t out_size,
		   const char *delimiter,
		   size_t del_len)
{
  CallbackData data;
  data.size=out_size;
  data.out=out;
  data.delimiter=delimiter;
  data.del_len=del_len;
  data.status=WC_RET_NORMAL;
  data.wordcut=self;
  wc_split(str,thai_callback_tag,&data,default_callback,&data);
  *data.out='\0';
  return data.status;
}


int 
wc_wordcut_result_len(WcWordcutResult *result)
{
  return result->n;
}

WC_STATUS
wc_wordcut_result_surface_at(WcWordcutResult *self,int p,char *out,size_t maxsize)
{
  int i=self->n-p-1,j,start,stop;
  assert(i<self->n);
  stop=self->index[i];
  start=self->tab[stop].start;
  if (maxsize-1 <= stop-start+1)
    return WC_RET_ERROR;
#ifdef DEBUG
  printf ("Start=%d Stop=%d i=%d\n",start,stop,i);
#endif
  for(j=start;j<=stop;j++) *out++ = self->str[j];
  *out='\0';
  return WC_RET_NORMAL;
}

const WcDictIterPos*
wc_wordcut_result_pos_at(WcWordcutResult *self,int p)
{
  int i=self->n-p-1;
  assert(i<self->n);
  return self->tab[self->index[i]].pos;
}

WcWordType
wc_wordcut_result_type_at(WcWordcutResult *self,int p)
{
  int i=self->n-p-1;
  assert(i<self->n);
  return self->tab[self->index[i]].type;
}
