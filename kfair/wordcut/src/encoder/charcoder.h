#ifndef __CHAR_CODER_H__
#define __CHAR_CODER_H__

typedef struct charcoder_t 
{
  char* array;
  size_t length,size;
} CharCoder;

CharCoder* char_coder_new();
void char_coder_init(CharCoder* self);
size_t char_coder_char_id(CharCoder* self,char ch,wc_boolean* found);
void char_coder_add_char(CharCoder* self,char ch);
void char_coder_add_str(CharCoder *self,const char* str); 
void char_coder_delete(CharCoder *self);
void char_coder_destroy(CharCoder *self);
char char_coder_char(CharCoder *self,size_t id,wc_boolean *found);
size_t char_coder_max(CharCoder *self);
#endif
