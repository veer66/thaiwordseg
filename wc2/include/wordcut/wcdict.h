#ifndef __WC_DICT_H__
#define __WC_DICT_H__

#define WC_DICT_ITER_ACTIVE     1
#define WC_DICT_ITER_DEAD       0

typedef int WcDictIterStatus;

typedef struct wc_dict_t {
  char **pos;
  wc_uint32 pos_size,root,size;
  wc_uchar *tab;
} WcDict;

typedef struct wc_dict_iter_t {
  WcDict* dict;
  wc_uint32 p;
  WcDictIterStatus status;
} WcDictIter;


WC_STATUS wc_dict_load(WcDict *self,const char *filename);
void wc_dict_init(WcDict *self);
void wc_dict_destroy(WcDict* self);
void wc_dict_delete(WcDict* self);
void wc_dict_root(WcDict *self,WcDictIter* iter);

#endif
