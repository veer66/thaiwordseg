#ifndef __WCWORDUNIT_H__
#define __WCWORDUNIT_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<wordcut/wccommon.h>

#define WC_WORDUNIT_NULL   -1

typedef struct wc_wordunit_map
{
  size_t len;
  int *assoc_tab,*break_tab;
} WcWordunitMap;

int wc_wordunit_map_assoc(WcWordunitMap *map,int stop);
wc_boolean wc_wordunit_map_break(WcWordunitMap *map,int start,int stop);
void wc_wordunit_map_delete(WcWordunitMap *self);
WcWordunitMap* wc_wordunit_find(const char* str,size_t len);
WcWordunitMap* wc_wordunit_map_new(const char* str,size_t len);
void wc_wordunit_map_init (WcWordunitMap *self,const char* str,size_t len);

#endif
