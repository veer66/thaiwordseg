#ifndef __WC_DICT_MAP_H__
#define __WC_DICT_MAP_H__

#include<wordcut/wcdict.h>

#define WC_DICT_MAP_NULL -1

typedef struct wc_dict_map_tok_t
{
  int start;
  WcDictIter iter;
}WcDictMapTok;

typedef struct wc_dict_map_tok_pos_t
{
  int start;
  WcDictIterPos pos;
}WcDictMapTokPos;

typedef struct wc_dict_map_t
{
  int* index;  
  WcDictMapTokPos *tok;
  size_t strlen;
  size_t len;
  size_t size;
} WcDictMap;

void wc_dict_map_init(WcDictMap *self,WcDict *dict,const char *str,size_t len);
void wc_dict_map_destroy(WcDictMap *self);
void wc_dict_map_delete(WcDictMap *self);
int wc_dict_map_assoc_len(WcDictMap *self,int stop);
int wc_dict_map_assoc_at(WcDictMap *self,int stop,int offset);
WcDictMap* wc_dict_map_new(WcDict *dict,const char *str,size_t len);
WcDictMap* wc_dict_get_map(WcDict *self,const char *str,size_t len);
const WcDictIterPos* wc_dict_map_assoc_pos_at(WcDictMap *self,int stop,int offset);

#endif
