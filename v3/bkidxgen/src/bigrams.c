#include "bigrams.h"


struct cal_data_t
{
  BiGrams *bi;
  WcWordcut *wordcut;
};


static void
eng_cb(const gchar* str,gsize len,gpointer data)
{
  struct cal_data_t *cdata=(struct cal_data_t *)data;
  BiGrams *bi=cdata->bi;
  bi_grams_stop(bi);
}

static void
thai_cb(const gchar* str,gsize len,gpointer data)
{
  struct cal_data_t *cdata=(struct cal_data_t *)data;
  BiGrams *bi=cdata->bi;
  WcWordcut *wordcut=cdata->wordcut;
  WcWordcutResult result;
  gint l,i;
  gchar *word;
  word=g_new(gchar,len+2);
  wc_wordcut_cut(wordcut,str,len,&result);
  l=wc_wordcut_result_len(&result);
  g_assert (l!=-1);
  for(i=0;i<l;i++)
    {
      WC_STATUS ret;
      ret=wc_wordcut_result_surface_at(&result,i,word,len+2);
      bi_grams_add(bi,word,strlen(word));
      //g_assert(ret!=WC_RET_ERROR);
    }
  g_free(word);

}

static gpointer
bigram_callback1(gchar *txt,gpointer data)
{
  wc_split(txt,thai_cb,data,eng_cb,data);
  return NULL;
}

static gpointer 
bigram_callback2(gchar *txt,gpointer data)
{
  BiGrams *bi=(BiGrams*)data;
  bi_grams_stop(bi);
  return NULL;
}



void
bi_grams_latex(const gchar* filename,BiGrams *bi,WcWordcut *wordcut)
{
  struct cal_data_t data;
  lxpa_init();
  lxpa_input(filename);
  data.bi=bi;
  data.wordcut=wordcut;
  lxpa_set_callback(NULL,NULL,
		    NULL,NULL, 
		    NULL,NULL,
		    NULL,NULL,
		    
		    NULL,NULL,
		    NULL,NULL,
		    NULL,NULL,
		    NULL,NULL,

		    bigram_callback1,(gpointer)&data,
		    bigram_callback2,(gpointer)bi,
		    bigram_callback2,(gpointer)bi);
  lxpa_parse();
  lxpa_destroy();
  bi_grams_cal(bi);
}


void 
bi_grams_init(BiGrams *self,gint th)
{
  self->uni=g_hash_table_new_full(g_str_hash,g_str_equal,g_free,g_free);
  self->bi=g_hash_table_new_full(g_str_hash,g_str_equal,g_free,g_free);
  self->val=g_hash_table_new_full(g_str_hash,g_str_equal,NULL,g_free);
  self->prev=NULL;
  self->n=0;
  self->th=th;
}

void
bi_grams_add(BiGrams *self,const gchar* str,gsize len)
{
  gchar *bikey,*unikey;
  gint *univ,*biv;
  self->n++;
  univ=(gint *)g_hash_table_lookup(self->uni,str);
  unikey=strdup(str);
  

  if(univ==NULL)
    {
      univ=g_new(gint,1);
      *univ=1;
      g_hash_table_insert(self->uni,unikey,univ);
    }
  else
    {
      (*univ)++;
    }
			  		       
  if (self->prev!=NULL)
    {
      bikey=g_new(gchar,self->prev_len+len+2);
      sprintf (bikey,"%s %s",self->prev,unikey);
      if((biv=(gint *)g_hash_table_lookup(self->bi,bikey))==NULL)
	{
	  biv=g_new(gint,1);
	  *biv=1;
	  g_hash_table_insert(self->bi,bikey,biv);
	}
      else
	{
	  g_free(bikey);
	  (*biv)++;
	}
    }

  self->prev=unikey;
  self->prev_len=len;
}


void
bi_grams_destroy(BiGrams *self)
{
  g_hash_table_destroy(self->uni);
  g_hash_table_destroy(self->bi);
  g_hash_table_destroy(self->val);
}

void
bi_grams_stop(BiGrams *self)
{
  self->prev=NULL;
}

static void 
cal(gpointer pkey,gpointer pval,gpointer pdata)
{
  gchar *key=(gchar *)pkey;
  BiGrams *bi=(BiGrams *)pdata;
  gint *freq=(gint *)pval;
  gdouble *v;

  /*  printf ("Key=-%s-\tfreq=%d\n",key,*freq); */
  
  if (*freq>bi->th)
    {
      gint *w1uni,*w2uni;
      gchar *p,*t,*w1,*w2;
      t=strdup(key);
      p=strchr(t,' ');
      if (p==NULL) g_error("Invalid string in bigrams.");
      *p='\0';
      w1=strdup(t);
      w2=strdup(p+1);
      
      w1uni=(gint *)g_hash_table_lookup(bi->uni,w1);
      if (w1uni==NULL) g_error("Invalid frequency.");
      w2uni=(gint *)g_hash_table_lookup(bi->uni,w2);
      if (w2uni==NULL) g_error("Invalid frequency.");
      
      v=g_new(gdouble,1);
      *v=log(((*freq)*bi->n)/((double)((*w1uni)*(*w2uni))))/log(2);
      g_hash_table_insert(bi->val,key,v);
      g_free(w1);
      g_free(w2);
      g_free(t);
    }
}

void 
bi_grams_cal(BiGrams *self)
{
  g_hash_table_foreach(self->bi,cal,self);
}

gdouble 
bi_grams_val(BiGrams *self,const gchar *w1,gsize w1l,
	     const gchar* w2,gsize w2l)
{
  gdouble *v;
  gchar* t;
  t=g_new(gchar,w2l+w1l+2);
  sprintf (t,"%s %s",w1,w2);
  v=g_hash_table_lookup(self->val,t);
  g_free(t);
  if (v==NULL) return 0;
  return *v;
}



static void
dump(gpointer p_key,gpointer p_val,gpointer p_data)
{
  printf ("%s\t%f\n",((gchar*)p_key),*((gdouble*)p_val));
}

void
bi_grams_dump(BiGrams *self)
{
  g_hash_table_foreach(self->val,dump,NULL);
}

