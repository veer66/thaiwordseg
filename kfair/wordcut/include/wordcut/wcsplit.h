#ifndef __WCSPLIT_H__
#define __WCSPLIT_H__
#include<stdlib.h>
typedef void (*wc_split_callback_t)(const char *,size_t,void*);
int wc_split (const char *source,
	     wc_split_callback_t thai_callback,
	     void* thai_support_data,
	     wc_split_callback_t default_callback,
	     void* default_support_data);
#endif
