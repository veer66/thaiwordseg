#include<wordcut/wccommon.h>
#include<stdio.h>


typedef struct wc_dict_t {
  char **pos;
  wc_uint32 pos_size,root,size;
  wc_uchar *tab;
} WcDict;


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
      printf ("%s\n",buffer);
    }
  p++;
  pos_len=self->tab[p];
  p++;
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

int 
main (int argc,char** argv) 
{
  if (argc!=2) WC_HALT("Invalid argument.");
   test_dump_wordlist(argv[1]); 
  return 0;
}

