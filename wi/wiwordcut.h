#ifndef __WI_WORDCUT_H__
#define __WI_WORDCUT_H__

#include "wicommon.h"
#include<stdlib.h>

typedef int *WiForest;
typedef int *WiPath;
typedef unsigned int *widict_t;
typedef widict_t WiDict;

typedef struct wi_wordcut {
  WiDict dict;
} WiWordcut;

typedef struct wi_result {
  WiPath path;
  char* str;
  size_t len;
} WiResult;

typedef struct wi_result_array {
  char** element;
  size_t size;
} WiResultArray;

WiWordcut* wi_wordcut_new(const char *dictfile);
void wi_wordcut_delete(WiWordcut *self);
void wi_wordcut_destroy(WiWordcut *self);
void wi_wordcut_init(WiWordcut *self,const char *dictfile);
WiAnswer* wi_wordcut_cut(WiWordcut *self,const char* str,size_t len);
void wi_result_delete(WiResult *self);
const char* wi_result_fetch_string(WiResult *result,const char* delim,size_t delim_len);

#endif
