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

typedef struct wi_answer {
  WiPath path;
  char* str;
  size_t len;
} WiAnswer;

WiWordcut* wi_wordcut_new(const char *dictfile);
void wi_wordcut_delete(WiWordcut *self);
void wi_wordcut_destroy(WiWordcut *self);
void wi_wordcut_init(WiWordcut *self,const char *dictfile);
WiAnswer* wi_wordcut_cut(WiWordcut *self,const char* str,size_t len);
void wi_answer_delete(WiAnswer *self);
const char* wi_answer_fetch_string(WiAnswer *answer,const char* delim,size_t delim_len);
#endif
