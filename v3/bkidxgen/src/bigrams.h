#ifndef __BI_GRAMS_H__
#define __BI_GRAMS_H__

#include<stdio.h>
#include<glib.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<wordcut/wcwordcut.h>
#include<latexparser/latexparser.h>
typedef struct bi_grams_t
{
  GHashTable *uni , *bi , *val;
  gchar* prev;
  gsize prev_len,n;
  gint th;
} BiGrams;


void bi_grams_init(BiGrams *self,gint th);
void bi_grams_add(BiGrams *self,const gchar* str,gsize len);
void bi_grams_destroy(BiGrams *self);
void bi_grams_stop(BiGrams *self);
void bi_grams_cal(BiGrams *self);
gdouble bi_grams_val(BiGrams *self,const gchar *w1,gsize w1l,
		     const gchar* w2,gsize w2l);
void bi_grams_dump(BiGrams *self);
void bi_grams_latex(const gchar* filename,BiGrams *bi,WcWordcut *wordcut);
#endif
