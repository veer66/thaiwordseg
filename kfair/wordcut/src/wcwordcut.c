#include <glib.h>
#include <wordcut/wcdict.h>
#include <wordcut/wcwordunit.h>
#include <wordcut/wcpath.h>
#include <wordcut/wcsplit.h>


#include <assert.h>


#define WC_WORD_TYPE_UNK      'U'
#define WC_WORD_TYPE_DICT     'D'
#define WC_WORD_TYPE_WORDUNIT 'W'

typedef gint WcWordType;

#define DEBUG 

typedef struct wc_wordcut_t
{
  WcDict *dict;  
} WcWordcut;

typedef struct wc_wordcut_table_element_t
{
  gint brk_c,unk_ch_c,wu_c,dict_c;
  /* 
   * brk_c  : Number of wordunit rule count
   * unk_ch : Number of characters in Unknown
   * wu_c   : Number of words from Wordunit
   * dict_c : Number of words from Dictionary
   */ 
  const WcDictIterPos* pos;
  WcWordType type;
  gint start;
} WcWordcutTableElement;


static gint 
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
	 gint start,gint stop,WcWordType wt,WcWordunitMap *wu_map)
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
select_path(WcDictMap *dict_map,WcWordunitMap *wu_map,
	    gchar ch,WcWordcutTableElement* tab,gint n)
{
  WcWordcutTableElement selected,canidate;
  gboolean first=TRUE;
  gint start,i,len;
  
  /* from wordunit */
  start=wc_wordunit_map_assoc(wu_map,n);
  if(start!=WC_WORDUNIT_NULL)
    {
      set_element(tab,&canidate,start-1,n,
		  WC_WORD_TYPE_WORDUNIT,wu_map);
      if (first || element_cmp(&selected,&canidate)>0)
	{
	  first=FALSE;
	  selected=canidate;
	  selected.pos=NULL;
	  selected.type=WC_WORD_TYPE_WORDUNIT;
	  selected.start=start;
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
	  first=FALSE;
	  selected=canidate;
	  selected.type=WC_WORD_TYPE_DICT;
 	  selected.pos=wc_dict_map_assoc_pos_at(dict_map,n,i);
	  assert(selected.pos!=NULL);
	  selected.start=start;
	}
    }

  /* unknown */
  set_element(tab,&canidate,n-1,n,WC_WORD_TYPE_UNK,wu_map);
  if(first || element_cmp(&selected,&canidate)>0)
    {
      first=FALSE;
      selected=canidate;
      selected.type=WC_WORD_TYPE_UNK;
      selected.pos=NULL;
      selected.start=n;
    }

  tab[n]=selected;
  
}

#ifdef DEBUG
static void
dump_tab(WcWordcutTableElement *tab,gsize len)
{
  gint i;
  for(i=0;i<len;i++)
    {
      printf ("i=%d start=%d break=%d unk=%d wordunit=%d dict=%d\n",
	      i,tab[i].start,tab[i].brk_c,tab[i].unk_ch_c,
	      tab[i].wu_c,tab[i].dict_c);
	      
    }
}
#endif

static WC_STATUS
tab2str(WcWordcutTableElement *tab,
	const gchar *src,gsize len,
	gchar *out,gsize outlen,const gchar* delimiter,gsize dimlen)
{
  gint i,start,j;
  gint *index;
  gsize n;
  gboolean first=TRUE;
  
  index=g_new(gint,len);
  i=len-1;
  n=0;
#ifdef DEBUG
  printf ("!!!1\n");
#endif
  while (i>=0)
    {
      start=tab[i].start;
      index[n]=i;    
      n++;
      i=start-1;
    }
#ifdef DEBUG
  printf ("!!!2\n");
#endif
  if (n<=0) return WC_RET_ERROR;
  if (dimlen*(n-1)+len+1>=outlen) return WC_RET_ERROR;
#ifdef DEBUG
  printf ("!!! n=%d\n",n);
#endif
  for(i=n-1;i>=0;i--)
    {
#ifdef DEBUG
      printf ("!!!3 start=%d stop=%d\n",tab[index[i]].start,index[i]);
#endif      
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

typedef struct wc_wordcut_result_t
{
  gsize len;
  gchar *str;
  WcWordcutTableElement *tab;
  gint *index;
  gint n;
} WcWordcutResult;

static void
dump_result(WcWordcutResult *self)
{
  gint i;
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

gint
wc_wordcut_result_str(WcWordcutResult* self , gchar *out , gsize out_size   ,
		      const gchar *delimiter,gsize del_len)
{
  gint i,j,l=0;
  gboolean first=TRUE;
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
	  first=FALSE;
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

static void
genidx(WcWordcutTableElement *tab,gsize len,gint **idx,gint *n)
{
  gint c=0,i,start;
  *idx=g_new(gint,len);
  for(i=len-1;i>=0;i=start-1)
    {
      start=tab[i].start;
#ifdef DEBUG
      printf ("!!! START=%d i=%d\n",start,i);
#endif
      (*idx)[c]=i;
      c++;
    }
  *n=c;
}

void
wc_wordcut_cut(WcWordcut *self,const gchar* str,gint len,WcWordcutResult *result)
{
  gint i;
  WcWordcutTableElement *tab;
  WcDictMap dict_map;
  WcWordunitMap wu_map;
  
#ifdef DEBUG
  #define MAXLEN 1024
  gchar out[MAXLEN];
#endif

  tab=g_new(WcWordcutTableElement,len);
  wc_wordunit_map_init(&wu_map,str,len);
  wc_dict_map_init(&dict_map,self->dict,str,len);
  for(i=0;i<len;i++)
    {
      select_path(&dict_map,&wu_map,str[i],tab,i);
    }
#ifdef DEBUG
  dump_tab(tab,len);
  tab2str(tab,str,len,out,MAXLEN,"|",1);
  printf ("Output = %s\n",out);
#endif
  result->tab=tab;
  result->len=len;
  result->str=g_strdup(str);
  genidx(tab,len,&result->index,&result->n);
}

void
wc_wordcut_result_destroy(WcWordcutResult *self)
{
  free(self->tab);
  free(self->str);
}

void 
wc_wordcut_init(WcWordcut *self,WC_STATUS *error)
{
  self->dict=wc_dict_new();
  if (wc_dict_load(self->dict,WC_SHARE_DATA_PATH "dict.etd")==WC_RET_NORMAL)
    *error=WC_RET_NORMAL;
  else
    *error=WC_RET_ERROR;

}

WcWordcut*
wc_wordcut_new()
{
  WcWordcut *self;
  WC_STATUS error;
  self=g_new(WcWordcut,1);
  wc_wordcut_init(self,&error);
  if (error==WC_RET_ERROR) return NULL;
  return self;
}

void 
wc_wordcut_destroy(WcWordcut *self)
{
  wc_dict_delete(self->dict);
}

void
wc_wordcut_delete(WcWordcut *self)
{
  wc_wordcut_destroy(self);
  free(self);
}

typedef struct
{
  gchar *out;
  gsize size;
  WC_STATUS status;
  const gchar *delimiter;
  gsize del_len;
  WcWordcut *wordcut;
} CallbackData;

void 
thai_callback(const gchar *str,gsize len,void *data)
{
  gint l;
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

void
default_callback(const gchar *str,gsize len,void *data)
{
   CallbackData *cb_data=(CallbackData *)data;
   gint j;
   if (cb_data->status==WC_RET_ERROR) return;
   if (len+1>=cb_data->size)
     {
       cb_data->status=WC_RET_ERROR;
       return;
     }
   for(j=0;j<len;j++)
     {
       *cb_data->out++ = str[j];
     }
   cb_data->size -= len;
}

WC_STATUS
wc_wordcut_cutline(WcWordcut *self,const gchar* str,
		   gchar *out,gsize out_size,
		   const gchar *delimiter,
		   gsize del_len)
{
  CallbackData data;
  data.size=out_size;
  data.out=out;
  data.delimiter=delimiter;
  data.del_len=del_len;
  data.status=WC_RET_NORMAL;
  data.wordcut=self;
  wc_split(str,thai_callback,&data,default_callback,&data);
  data.out='\0';
  return data.status;
}

gint 
wc_wordcut_result_len(WcWordcutResult *result)
{
  return result->n;
}

WC_STATUS
wc_wordcut_result_surface_at(WcWordcutResult *self,gint p,gchar *out,gsize maxsize)
{
  gint i=self->n-p-1,j,start,stop;
  assert(i<self->n);
  stop=self->index[i];
  start=self->tab[stop].start;
  if (maxsize-1 <= stop-start+1)
    return WC_RET_ERROR;
  printf ("Start=%d Stop=%d i=%d\n",start,stop,i);
  for(j=start;j<=stop;j++) *out++ = self->str[j];
  *out='\0';
  return WC_RET_NORMAL;
}

WC_STATUS
wc_wordcut_result_pos_str_at(WcWordcutResult *self,gint p,gchar *out,gsize maxsize)
{
  gint i=self->n-p-1,j,start,stop;
  assert(i<self->n);
  stop=self->index[i];
  start=self->tab[stop].start;
  if (maxsize-1 <= stop-start+1)
    return WC_RET_ERROR;
  printf ("Start=%d Stop=%d i=%d\n",start,stop,i);
  for(j=start;j<=stop;j++) *out++ = self->str[j];
  *out='\0';
  return WC_RET_NORMAL;
}


int 
main(int argc,char **argv)
{
#define MAX_SIZE 1024
  WcWordcut wordcut;
  WC_STATUS error;
  WcWordcutResult result;
  gchar out[MAX_SIZE];
  gchar *str;
  gint n;
  if (argc!=3) 
    {
      printf ("Invalid argument.!\n");
      exit(1);
    }

  wc_wordcut_init(&wordcut,&error);
  if (error==WC_RET_ERROR)
    {
      printf ("Initialize Wordcut Error.\n");
      exit(1);
    }
  str=argv[1];
  n=atoi(argv[2]);
  wc_wordcut_cut(&wordcut,str,strlen(str),&result);
  printf ("Len=%d\n",wc_wordcut_result_len(&result));
  wc_wordcut_result_surface_at(&result,n,out,MAX_SIZE);
  printf ("Out = %s\n",out);
  wc_wordcut_destroy(&wordcut);
  return 0;
}

