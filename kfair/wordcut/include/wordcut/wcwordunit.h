#ifndef __WCWORDUNIT_H__
#define __WCWORDUNIT_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<glib.h>

#define WC_WORDUNIT_NULL   -1

typedef struct wc_wordunit_map
{
  gsize len;
  gint *assoc_tab,*break_tab;
} WcWordunitMap;

gint wc_wordunit_map_assoc(WcWordunitMap *map,gint stop);
gboolean wc_wordunit_map_break(WcWordunitMap *map,gint start,gint stop);
void wc_wordunit_map_delete(WcWordunitMap *self);
WcWordunitMap* wc_wordunit_find(const gchar* str,gsize len);
WcWordunitMap* wc_wordunit_map_new(const gchar* str,gsize len);
void wc_wordunit_map_init (WcWordunitMap *self,const gchar* str,gsize len);

#endif
