#ifndef __WC_WORDCUT_H__
#define __WC_WORDCUT_H__


#include <wordcut/wcdict.h>
#include <wordcut/wcdictmap.h>
#include <wordcut/wcwordunit.h>
#include <wordcut/wcpath.h>
#include <wordcut/wcsplit.h>

#define WC_WORD_TYPE_UNK      'U'
#define WC_WORD_TYPE_DICT     'D'
#define WC_WORD_TYPE_WORDUNIT 'W'
#define WC_WORD_TYPE_JOIN     'J'

typedef int WcWordType;

typedef struct wc_wordcut_t
{
  WcDict *dict;  
  wc_boolean ext_dict;
} WcWordcut;

typedef struct wc_wordcut_table_element_t
{
  int brk_c,unk_ch_c,wu_c,dict_c;
  /* 
   * brk_c  : Number of wordunit rule count
   * unk_ch : Number of characters in Unknown
   * wu_c   : Number of words from Wordunit
   * dict_c : Number of words from Dictionary
   */ 
  const WcDictIterPos* pos;
  WcWordType type;
  int start;
} WcWordcutTableElement;

typedef struct wc_wordcut_result_t
{
  int n;
  int* index;
  int* reverse;
  size_t len;
  char *str;
  WcWordcutTableElement *tab;
} WcWordcutResult;

int wc_wordcut_result_str(WcWordcutResult* self , char *out , size_t out_size , const char *delimiter,size_t del_len);

void wc_wordcut_cut(WcWordcut *self,const char* str,int len,WcWordcutResult *result);
void wc_wordcut_result_destroy(WcWordcutResult *self);
void wc_wordcut_init(WcWordcut *self,WC_STATUS *error);
WcWordcut* wc_wordcut_new();
void wc_wordcut_destroy(WcWordcut *self);
void wc_wordcut_delete(WcWordcut *self);
WC_STATUS wc_wordcut_cutline(WcWordcut *self,const char* str,
  char *out,size_t out_size,const char *delimiter,size_t del_len);
int wc_wordcut_result_len(WcWordcutResult *result);
WC_STATUS wc_wordcut_result_surface_at(WcWordcutResult *self,int p,char *out,size_t maxsize);
const WcDictIterPos* wc_wordcut_result_pos_at(WcWordcutResult *self,int p);
WcWordType wc_wordcut_result_type_at(WcWordcutResult *self,int p);
void wc_wordcut_result_destroy(WcWordcutResult *self);
WcWordcut* wc_wordcut_new_with_dict(WcDict *dict);
void wc_wordcut_init_with_dict(WcWordcut *self,WcDict *dict);


#endif
