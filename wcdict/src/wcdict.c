#include<wordcut/wccommon.h>
#include<stdio.h>
#include<wordcut/wcdict.h>



static void
dump_iterator_info(WcDictIter *self)
{
  printf ("Dict=0x%X\n",(unsigned int)self->dict);
  printf ("P=0x%X\tI=%d\n",self->p,self->i);
  printf ("Type=");
  switch (self->type)
    {
    case WC_NODE_TYPE_SEQ: printf ("SEQ"); break;
    case WC_NODE_TYPE_BIN: printf ("BIN"); break;
    case WC_NODE_TYPE_POS: printf ("POS"); break;
    default: printf ("ERR"); break;
    }
  printf ("\tterminator=%c\n",self->terminator ? 'T':'F');
  printf ("Status=");
  switch(self->status)
    {
    case WC_DICT_ITER_ACTIVE:printf("ACTIVE"); break;
    case WC_DICT_ITER_DEAD: printf("DEAD"); break;
    default: printf ("ERR"); break;
    }
  printf ("\n");
}

static WC_STATUS
read_pos(WcDict *self,FILE *fp)
{
  int count;
  wc_uchar len;
  WC_RET_CHECK(fread(&self->pos_size,sizeof(wc_uint32),1,fp)==1);
  self->pos=WC_NEW_N(char *,self->pos_size);
  for (count=0;count < self->pos_size;count++)
    {
      WC_RET_CHECK(fread(&len,1,1,fp)==1);
      self->pos[count]=WC_NEW_STR(len);
      WC_RET_CHECK(fread(self->pos[count],1,len,fp)==len);
      self->pos[count][len]='\0';
    }
  return WC_RET_NORMAL;
}

static WC_STATUS
read_tab(WcDict *self,FILE *fp)
{
  WC_RET_CHECK(fread(&self->root,4,1,fp)==1);
  WC_RET_CHECK(fread(&self->size,4,1,fp)==1);
  self->tab=WC_NEW_N(wc_uchar,self->size);
  WC_RET_CHECK(fread(self->tab,1,self->size,fp)==
	       self->size);
  return WC_RET_NORMAL;
}

static void
dump_node(WcDict *self,wc_uint32 p,int level)
{
  wc_uchar type;
  wc_byte pos_len;
  wc_byte child_count;
  wc_byte len;
  wc_uint32 child;
 
  int i;
 
  type=(self->tab[p] & 0xF0) >> 4;
  printf ("Node Pos = 0x%X\tLevel=%d\n",p,level);
  printf ("Type = ");
  switch (type)
    {
    case 1:
      printf ("Binary");
      break;
    case 2:
      printf ("Pos Only");
      break;
    case 3:
      printf ("Compressed");
      break;
    default:
      printf ("Error");
      WC_HALT("Error dumping node");
      break;
    }
  printf ("\n");
  p++;
  pos_len=self->tab[p];

  p++;
  printf ("Pos len=%d --> ",pos_len);
  for(i=0;i<pos_len;i++)
    {
      printf ("%s ",self->pos[self->tab[p+i]]);
    }
  printf ("\n");
  p=p+pos_len;
  switch (type)
    {
    case 1:
      /* Binary */
      child_count=self->tab[p];
      p++;
      for(i=0;i<child_count;i++)
	{
	  printf ("%c ",self->tab[p+i]);	  
	}
      printf ("\n");
      
      
      p=p+child_count;
      printf ("-----------------------------------\n");
      for(i=0;i<child_count*3;i++)
	{
	  printf ("%2X ",self->tab[p+i]);
	}
      printf ("\n");
      for(i=0;i<child_count;i++)
	{
	  printf ("Child = 0x%X\n",
		  child=(*(wc_uint32 *)&self->tab[p+i*3] & 0x00FFFFFF));     
	  dump_node(self,child,level+1);
	}
      printf ("+++++++++++++++++++++++++++++++++++\n");
      break;
    case 2:
      
      /* POS Only */
      break;
    case 3:
      /* Compressed */ 
      len=self->tab[p];
      p++;
      for(i=0;i<len;i++)
	{
	  printf ("%c",self->tab[p+i]);
	}
      printf ("\n");
      p=p+i;

      printf ("-----------------------------------\n");
      printf ("Child = 0x%X\n",child=(*(wc_uint32 *)&self->tab[p] & 0x00FFFFFF));
      dump_node(self,child,level+1);
      printf ("+++++++++++++++++++++++++++++++++++\n");
      break;
    default:
      /* Error */
      break;
    }
}

static void
wc_dict_dump_node(WcDict *self)
{
  dump_node(self,self->root,0);
}

#define MAX_BUFFER 1024


static void
dump_wordlist(WcDict *self,char *buffer,wc_uint32 p,int level)
{
  wc_boolean terminate;
  wc_uchar type;
  wc_byte pos_len,len;
  int i;
  wc_uint32 child;
  
  terminate = self->tab[p] & 0xF;
  type = self->tab[p] & 0xF0;
  if (terminate)
    {
      /* print word */
      buffer[level] = '\0';
      printf ("%s",buffer);
    }
  p++;
  pos_len=self->tab[p];
  p++;
  if (terminate)
    {
      for(i=0;i<pos_len;i++)
	{
	  printf ("\t%s",self->pos[self->tab[p+i]]);
	}
      printf ("\n");
    }
  p=p+pos_len;
  switch (type)
    {
    case 0x10: /*binary*/
      len=self->tab[p];
      if (len>0)
	{
	  p++;
	  for(i=0;i<len;i++)
	    {
	      buffer[level]=self->tab[p+i];
	      child=*(wc_uint32 *)&self->tab[p+len+i*3] & 0x00FFFFFF;
	      dump_wordlist(self,buffer,child,level+1);
	    }
	}
      break;
    case 0x20: /* pos only */
      break;
    case 0x30: /* compressed suffix */
      len=self->tab[p];
      if(len>0)
	{
	  p++;
	  for(i=0;i<len;i++)
	    {
	      buffer[level+i]=self->tab[p+i];
	    }
	  p=p+len;
	  child=*(wc_uint32 *)&self->tab[p] & 0x00FFFFFF;
	  dump_wordlist(self,buffer,child,level+len);
	}
      break;
    default:
      WC_HALT("Type error.");
      break;
    }
}

static void
wc_dict_dump_wordlist(WcDict *self)
{
  char buffer[MAX_BUFFER];
  dump_wordlist(self,buffer,self->root,0);
}

WC_STATUS
wc_dict_load(WcDict *self,const char *filename)
{
  FILE *fp;
  fp=fopen(filename,"r");
  WC_RET_CHECK(fp!=NULL);
  WC_RET_CHECK(read_pos(self,fp)==WC_RET_NORMAL);
  WC_RET_CHECK(read_tab(self,fp)==WC_RET_NORMAL);
  fclose(fp);
  return WC_RET_NORMAL;
}

void 
wc_dict_init(WcDict *self)
{
  self->tab=NULL;
  self->pos=NULL;
}

WcDict* wc_dict_new()
{
  WcDict* self=WC_NEW(WcDict);
  wc_dict_init(self);
  return self;
}


void 
wc_dict_destroy(WcDict* self)
{
  int i;
  for(i=0;i<self->pos_size;i++)
    {
      free(self->pos[i]);
    }
  free(self->pos);
  if (self->tab!=NULL) free(self->tab);
  
}

void 
wc_dict_delete(WcDict* self)
{
  wc_dict_destroy(self);
  free(self);
}


static void 
test_dump_node(const char dictfile[])
{
  WcDict *dict=wc_dict_new();
  if (wc_dict_load(dict,dictfile)!=WC_RET_NORMAL) 
    WC_HALT("Reading POS Error");
  wc_dict_dump_node(dict);
  wc_dict_delete(dict);
}

static void
test_dump_wordlist(const char dictfile[])
{
  WcDict *dict=wc_dict_new();
  if (wc_dict_load(dict,dictfile)!=WC_RET_NORMAL) 
    WC_HALT("Reading POS Error");
  wc_dict_dump_wordlist(dict);
  wc_dict_delete(dict);
}

void
wc_dict_root(WcDict *self,WcDictIter* iter)
{
  iter->p=self->root;
  iter->dict=self;
  iter->status=WC_DICT_ITER_ACTIVE;
  iter->terminator=WC_FALSE;
  iter->type=WC_NODE_TYPE_START;
  iter->i=0;
}



static int 
bin_search(wc_uchar *tab,size_t start,size_t len,wc_uchar ch)
{
  wc_uchar *t,tch;
  int l,r,m;
  wc_boolean found;
  t=tab+start;
  l=0;
  r=len-1;
  found=WC_FALSE;
  while(l<=r && l>=0 && r<len)
    {
      m=((l+r)>>1);
      tch=t[m];
      assert (l>=0 && r<len);
      if (ch < tch) 
	{
	  r=m-1;
	}
      else if (ch > tch)
	{
	  l=m+1;
	}
      else
	{
#ifdef WC_DEBUG
	  printf ("start=%d\tt[m]=%c\tm=%d\tch=%c\n",start,t[m],m,ch);
#endif
	  found=WC_TRUE;
	  break;
	}
    }
  return found ? m : -1;
}


static wc_uint32
get_child(const wc_uchar* tab,int p)
{		  
#ifdef WC_DEBUG
  printf ("Child P = %d , Child=%d\n",p,( *(wc_uint32 *)&tab[p]) & 0x00FFFFFF);
#endif 
  return  ((*(wc_uint32 *)&tab[p]) & 0x00FFFFFF);
}

WC_STATUS
wc_dict_iter_transit(WcDictIter *self,char ch)
{
  int t;
  WcDict *dict;
  wc_uchar type;
  wc_uint32 p;
  wc_uchar uch=(wc_uchar)ch;
  wc_uchar pos_len,len;


  dict=self->dict;
  p=self->p;
  type=(dict->tab[p] & 0xF0);
  p++;
  pos_len=dict->tab[p];
  p=p+pos_len+1; /* sizeof(poslen+terminator+type)=2 */
  switch (type)
    {
    case 0x10: /*binary*/
#ifdef WC_DEBUG
      printf ("BIN\n");
#endif
      self->type=WC_NODE_TYPE_BIN;
      len=dict->tab[p];
      if (len <= 0) return WC_RET_ERROR;
      p++;
      if((t=bin_search(dict->tab,p,len,uch))!=(-1))
	{
	  wc_uint32 child;
	  self->status=WC_DICT_ITER_ACTIVE;
	  child=get_child(dict->tab,p+len+t*3);
	  self->p=child;
#ifdef WC_DEBUG
	  printf ("T=%d\tP=%d\tlen=%d\tChild=%d\n",t,p,len,child);
#endif
	  self->terminator=dict->tab[child] & 0xF;
	}
      else
	{
	  self->status=WC_DICT_ITER_DEAD;
	  return WC_RET_NORMAL;
	}
      break;
    case 0x20: /* pos only */
#ifdef WC_DEBUG
      printf ("POS ONLY\n");
#endif
      self->type=WC_NODE_TYPE_POS;
      self->status=WC_DICT_ITER_DEAD;
      return WC_RET_NORMAL;
      break;
    case 0x30: /* compressed suffix */
#ifdef WC_DEBUG
      printf ("Compressed Suffix\n");
#endif
      if (self->type!=WC_NODE_TYPE_SEQ)
	{
	  self->i=0;
	  self->type=WC_NODE_TYPE_SEQ;
	}
      len=dict->tab[p];      
      p++;
      assert(self->i<len);
      if(len>0)
	{
	  if (dict->tab[self->i+p]==uch)
	    {
	      /* match */
	      self->i++;
	      if (self->i==len)
		{
		  /* goto child */
		  wc_uint32 child;
		  child=get_child(dict->tab,p+len);
		  self->terminator=dict->tab[child] & 0xF;
		  self->p=child;
		}
	    }
	  else
	    {
	      self->status=WC_DICT_ITER_DEAD;
	      return WC_RET_NORMAL;
	    }
	}
      break;
    default:
      return WC_RET_ERROR;
      break;
    }  

  
  return WC_RET_NORMAL;
}

static void 
dump_tab(unsigned char* tab,size_t tab_size)
{
  int i;
  for(i=0;i<tab_size;i++)
    {
      printf ("%d,0x%X\t",i,tab[i]);
      if (i % 10 == 0)
	{
	  printf ("\n");
	}
    }
  printf ("\n");
}

static void
test_iter(const char* filename,const char *str)
{
  WcDict* dict;
  WcDictIter iter;
  int i=0;
  dict=wc_dict_new();
  wc_dict_load(dict,filename);
  wc_dict_root(dict,&iter);
  printf ("Pos size = %d\n",dict->pos_size);
  printf ("Root = %d\n",dict->root);
  dump_tab(dict->tab,dict->size);
  while(iter.status!=WC_DICT_ITER_DEAD && str[i] != '\0')
    {
      printf ("--------------------->i=%d\n",i);
      dump_iterator_info(&iter);
      wc_dict_iter_transit(&iter,str[i]);
      i++;
    }
  wc_dict_delete(dict);
}

void
wc_dict_iter_pos(WcDictIter *self,WcDictIterPos *pos)
{
  pos->dict=self->dict;
  pos->p=self->p+2;
  pos->len=pos->dict->tab[self->p+1];  
}


WC_STATUS 
wc_dict_iter_pos_posid(WcDictIterPos *self,int index,wc_byte *id)
{
  if (index < self->len)
    {
      *id=self->dict->tab[self->p+index];
      return WC_RET_NORMAL;
    }
  return WC_RET_ERROR;
}


const char* 
wc_dict_iter_pos_posid2str(WcDictIterPos* self,wc_byte pos_id)
{
  WcDict *dict=self->dict;
  if (dict->pos_size <= pos_id) return NULL;
  return dict->pos[pos_id];
}



/*
int 
main (int argc,char** argv) 
{
  if (argc!=3) WC_HALT("Invalid argument.");
  test_iter(argv[1],argv[2]); 

  return 0;
}

*/
