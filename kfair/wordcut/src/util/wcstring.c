#include <wordcut/wcstring.h>
#include <string.h>

WcString*
wc_string_new_with_str(const char *str) 
{
  WcString *self=WC_NEW(WcString);
  wc_string_init_with_str(self,str);
  return self;
}

WcString*
wc_string_new_with_str_len(const char*str,size_t len)
{
  WcString *self=WC_NEW(WcString);
  wc_string_init_with_str_len(self,str,len);
  return self;
}

WcString*
wc_string_new_with_size(size_t size)
{
  WcString *self=WC_NEW(WcString);
  wc_string_init_with_size(self,size);
  return self;
}

void
wc_string_init_with_str(WcString *self,const char *str) 
{
  wc_string_init_with_str_len(self,str,strlen(str));
}

void
wc_string_init_with_str_len(WcString *self,const char *str,size_t len) 
{
  size_t i;
  wc_string_init_with_size(self,len);
  self->length=len;
  for (i=0;i<len;i++) 
    {
      assert (str[i] != '\0');
      self->data[i]=str[i];
    }
  self->data[i]='\0';
}

void 
wc_string_init_with_size(WcString *self,size_t size)
{
  self->data=WC_NEW_STR(size);
  self->length=0;
  self->size=size;
}

void 
wc_string_destroy(WcString *self)
{
  free(self->data);
}

void 
wc_string_delete(WcString *self)
{
  wc_string_destroy(self);
  free(self);
}
