#ifndef __STR_CODER_H__
#define __STR_CODER_H__
#include<wordcut/wccommon.h>
typedef struct str_coder_t {
  char **array;
  size_t size,length;
} StrCoder;


void str_coder_init(StrCoder *self,size_t max);
StrCoder* str_coder_new(size_t max);
void str_coder_destroy(StrCoder *self);
void str_coder_delete(StrCoder *self);
void str_coder_add(StrCoder *self,const char *str);
size_t str_coder_id(StrCoder *self,const char *id,wc_boolean *found);
const char* str_coder_str(StrCoder *self,size_t id);

#endif
