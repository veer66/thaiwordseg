#ifndef __WC_DICT_H__
#define __WC_DICT_H__
#include <wordcut/wccommon.h>

typedef struct {
  char **pos;
  wc_uint32 pos_size,root,size;
  wc_uchar *tab;
} WcDict;

WC_STATUS wc_dict_load(WcDict *self,const char *filename);
void wc_dict_init(WcDict *self);
void wc_dict_destroy(WcDict* self);
void wc_dict_delete(WcDict* self);
WcDict* wc_dict_new();

typedef int WcDictIterStatus;
typedef int WcNodeType;

#define WC_NODE_TYPE_SEQ             0
#define WC_NODE_TYPE_BIN             1
#define WC_NODE_TYPE_POS             2
#define WC_NODE_TYPE_START           3

#define WC_DICT_ITER_ACTIVE   0
#define WC_DICT_ITER_DEAD     1

#define WC_DICT_ITER_I_RESET  0
#define WC_DICT_ITER_I_INUSE  1

typedef struct wc_dict_iter_t {
  WcDict* dict;
  wc_uint32 p,i;
  int i_state;
  wc_boolean terminator;
  WcDictIterStatus status;
#ifdef WC_ITER_EXTRA_INFO
  WcNodeType type;
#endif
} WcDictIter;


void wc_dict_root(WcDict *self,WcDictIter* iter);
WC_STATUS wc_dict_iter_transit(WcDictIter *self,char ch);

typedef struct wc_dict_iter_pos 
{
  wc_uint32 p;
  wc_byte len;
  WcDict *dict;
} WcDictIterPos;

void wc_dict_iter_pos(WcDictIter *self,WcDictIterPos *pos);
const char* wc_dict_iter_pos_posid2str(WcDictIterPos* self,wc_byte pos_id);
WC_STATUS wc_dict_iter_pos_posid(WcDictIterPos *self,int index,wc_byte *id);


#endif
