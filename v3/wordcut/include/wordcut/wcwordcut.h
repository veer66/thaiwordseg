#ifndef __WC_WORDCUT_H__
#define __WC_WORDCUT_H__

#include <glib.h>
#include <wordcut/wcdict.h>
#include <wordcut/wcwordunit.h>
#include <wordcut/wcpath.h>
#include <wordcut/wcsplit.h>

#define WC_WORD_TYPE_UNK      'U'
#define WC_WORD_TYPE_DICT     'D'
#define WC_WORD_TYPE_WORDUNIT 'W'

typedef gint WcWordType;

typedef struct wc_wordcut_t
{
  WcDict *dict;  
} WcWordcut;

typedef struct wc_wordcut_table_element_t
{
  gint brk_c,unk_ch_c,wu_c,dict_c;
  /* 
   * brk_c  : Number of wordunit rule count
   * unk_ch : Number of characters in Unknown
   * wu_c   : Number of words from Wordunit
   * dict_c : Number of words from Dictionary
   */ 
  const WcDictIterPos* pos;
  WcWordType type;
  gint start;
} WcWordcutTableElement;

typedef struct wc_wordcut_result_t
{
  gsize len;
  gchar *str;
  WcWordcutTableElement *tab;
  gint *index;
  gint n;
} WcWordcutResult;

gint wc_wordcut_result_str(WcWordcutResult* self , gchar *out , gsize out_size , const gchar *delimiter,gsize del_len);

void wc_wordcut_cut(WcWordcut *self,const gchar* str,gint len,WcWordcutResult *result);
void wc_wordcut_result_destroy(WcWordcutResult *self);
void wc_wordcut_init(WcWordcut *self,WC_STATUS *error);
WcWordcut* wc_wordcut_new();
void wc_wordcut_destroy(WcWordcut *self);
void wc_wordcut_delete(WcWordcut *self);
WC_STATUS wc_wordcut_cutline(WcWordcut *self,const gchar* str,
  gchar *out,gsize out_size,const gchar *delimiter,gsize del_len);
gint wc_wordcut_result_len(WcWordcutResult *result);
WC_STATUS wc_wordcut_result_surface_at(WcWordcutResult *self,gint p,gchar *out,gsize maxsize);
const WcDictIterPos* wc_wordcut_result_pos_at(WcWordcutResult *self,gint p);
WcWordType wc_wordcut_result_type_at(WcWordcutResult *self,gint p);



#endif
