#ifndef __WC_STRING_H__
#define __WC_STRING_H__

#include <wordcut/wccommon.h>

typedef struct wc_string_t {
  char *data;
  size_t length,size;
} WcString;

WcString* wc_string_new_with_str(const char *str);
WcString* wc_string_new_with_str_len(const char*str,size_t len);
WcString* wc_string_new_with_size(size_t size);
void wc_string_init_with_str(WcString *self,const char *str);
void wc_string_init_with_str_len(WcString *self,const char *str,size_t len);
void wc_string_init_with_size(WcString *self,size_t size);
void wc_string_destroy(WcString *self);
void wc_string_delete(WcString *self);

#endif
