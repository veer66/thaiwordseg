#include <wordcut/wccommon.h>
#include "charcoder.h"


#define DEFAULT_ARRAY_SIZE    0x100;
size_t 
char_coder_max(CharCoder *self)
{
  return self->length;
}

CharCoder*
char_coder_new() 
{
  CharCoder *self=WC_NEW(CharCoder);
  char_coder_init(self);
  return self;
}

void
char_coder_init(CharCoder* self)
{
  self->size=DEFAULT_ARRAY_SIZE;
  self->array=WC_NEW_N(char,self->size);
  self->length=0;
}

size_t
char_coder_char_id(CharCoder* self,char ch,wc_boolean* found)
{
  size_t i,ans;
  *found=WC_FALSE;
  for (i=0;i<self->length && !(*found);i++) 
    {
      if(self->array[i]==ch) 
	{
	  *found=WC_TRUE;
	  ans=i;
	}
    }
  return (*found) ? ans : 0;
}

void
char_coder_add_char(CharCoder* self,char ch)
{
  wc_boolean found;
  size_t p;
  p=char_coder_char_id(self,ch,&found);
  if (!found) 
    {
      /* just add it */
      if (self->length+1 > self->size) 
	WC_HALT("too many characters in character coder.");
      self->array[self->length]=ch;
      self->length++;	  
    }
}

void 
char_coder_add_str(CharCoder *self,const char* str) 
{
  const char *p;
  for(p=str;*p!='\0';p++) 
    {
      char_coder_add_char(self,*p);
    }
}

void 
char_coder_destroy(CharCoder* self)
{
  free(self->array);
}

void
char_coder_delete(CharCoder *self)
{
  char_coder_destroy(self);
  free(self);
}

char
char_coder_char(CharCoder *self,size_t id,wc_boolean *found)
{
  if ((*found= (id < self->length)))
    {
      return self->array[id];
    }
  return '\0';
}
