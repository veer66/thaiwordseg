#include"strcoder.h"

void
str_coder_init(StrCoder *self,size_t max) 
{
  self->array=WC_NEW_N(char *,max);
  self->size=max;
  self->length=0;
}

StrCoder*
str_coder_new(size_t max) 
{
  StrCoder *self=WC_NEW(StrCoder);
  str_coder_init(self,max);
  return self;
}


void 
str_coder_destroy(StrCoder *self)
{
  free(self->array);
}

void
str_coder_delete(StrCoder *self)
{
  str_coder_destroy(self);
  free(self);
}

void
str_coder_add(StrCoder *self,const char *str)
{
  wc_boolean found=WC_FALSE;
  size_t i;
  for(i=0;i<self->length && !found;i++)
    {
      if(strcmp(str,self->array[i])==0) 
	{
	  found=WC_TRUE;
	}
    }
  if (!found) 
    {  
      if(self->length+1 > self->size) 
	WC_HALT("Too many code in coder.");
      WC_STR_DUP(self->array[self->length],str);
      self->length++;
    }
}

const char*
str_coder_str(StrCoder *self,size_t id)
{
  if (id >= self->length) return NULL;
  return self->array[id];
}

size_t
str_coder_id(StrCoder *self,const char *str,wc_boolean *found)
{

  size_t i,ans=0;
  *found=WC_FALSE;

  for(i=0;i<self->length && !*found;i++)
    {

      if(strcmp(str,self->array[i])==0) 
	{

	  *found=WC_TRUE;
	  ans=i;
	}
    }
#ifdef WC_DEBUG
  printf ("STR = %s found=%s Ans=%d\n",str,*found ? "true" : "false",ans);
#endif
  return ans;
}

