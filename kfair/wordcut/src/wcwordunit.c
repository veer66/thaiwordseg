
#include<wordcut/wcwordunit.h>

#define NOT_FOUND -1
#define BACKGROUND 0
#define DUMMY -1

static gint 
group_karun(const gchar* p,gsize len)
{
  if (len >= 4)
    {
      if (p[3]=='ì') return 3;
    }
  return NOT_FOUND;
}

static gint
group_follow_vowel(const gchar* p,gsize len)
{
  if (len>=2)
    {
      switch (p[1]) 
	{
	case 'Ò':
	case 'Ð':
	case 'Ô':
	case 'Õ':
	case 'Ö':
	case 'Ø':
	case 'Ù':
	  return 1;
	  break;
	}
    }
  return NOT_FOUND;
}

static gint 
group_tone(const gchar *p,gsize len)
{
  if (len>=2)
    {
      switch (p[1]) 
	{
	case 'è':
	case 'é':
	case 'ê':
	case 'ë':
	  return 1;
	}
    }
  return NOT_FOUND;
}

static gint
group_maihun(const gchar *p,gsize len)
{
  if (len>=3)
    {
      if(p[1]=='Ñ')
	{
	  if(len>=4 && (p[2]=='è' || p[2]=='é' || p[2]=='ê' || p[3]=='ë'))
	    return 3;
	  return 2;
	}
    }
  return NOT_FOUND;
}

static gint
group_lead_vowel(const gchar *p,gsize len)
{
  if(len>=2)
    {
      if(p[1]>='¡' && p[1]<='Î')
	{
	  switch (p[0])
	    {
	    case 'à':
	    case 'á':
	    case 'â':
	    case 'ä':
	    case 'ã':
	      return 1;
	    }
	}
    }
  return NOT_FOUND;
}

static gint
group_ko(const char *p,size_t len)
{
  if (len>=2)
    {
      if (p[0]=='¡' && p[1] =='ç')
	{
	  return 1;
	}
    }
  return NOT_FOUND;
}

static gint
group_ad(const gchar *p,gsize len)
{
  if (len>=4)
    {
      if (p[0]=='à' && p[1]!='¡' && p[2]=='ç')
	{
	  return 3;
	}
    }
  return NOT_FOUND;
}


static void
dump_tab(gint tab[],gsize len)
{
  gint i;
  for(i=0;i<len;i++)
    {
      g_print("%d",tab[i]);
    }
  g_print("\n");
}

static void 
marktab(gint tab[],gint *c_marker,gint start,gint offset,gsize len)
{

  gint stop,marker,i;
  stop=start+offset;

  (*c_marker)++;

  if(tab[start]!=BACKGROUND)
    marker=tab[start];
  else
    marker=(*c_marker);

  /* 
   * at tab[stop] need more than naive mark 
   * it will perform after this for loop
   */
  for(i=start;i<stop;i++) tab[i]=marker;

  if(tab[stop]!=BACKGROUND)
    {
      gint old_marker=tab[stop];
      gint j;
      for(j=stop;j<len && tab[j]==old_marker;j++)
	tab[j]=marker;      
    }
  else
    {
      tab[stop]=marker;
    }
}




gint*
tab2map(gint tab[],gsize len)
{
  gint i,prev=BACKGROUND,start=DUMMY,stop=DUMMY;
  
  gint *map=g_new(gint,len);
  for(i=0;i<len;i++) map[i]=WC_WORDUNIT_NULL;
  for(i=0;i<len;i++) 
    {
      if(prev!=tab[i])
	{
	  stop=i;
	  if(start!=DUMMY && prev!=BACKGROUND)
	    map[stop-1]=start;
	  start=i;
	  prev=tab[i];
	}
    }
  if(prev!=BACKGROUND) map[len-1]=start;
  return map;
}

gint
wc_wordunit_map_assoc(WcWordunitMap *map,gint stop)
{
  if (stop>=map->len) return WC_WORDUNIT_NULL;
  return map->assoc_tab[stop];
}

gboolean
wc_wordunit_map_break(WcWordunitMap *map,gint start,gint stop)
{
  if (start<=stop && stop<map->len)
    {
      if(map->break_tab[stop]==map->break_tab[start]
	 && map->break_tab[stop]!=BACKGROUND)	
	{
	  if (start!=0 && map->break_tab[start]==map->break_tab[start-1])
	    {
	      return TRUE;
	    }
	  else if (stop!=map->len-1 && map->break_tab[stop]==
		   map->break_tab[stop+1])
	    {
	      return TRUE;
	    }
	  
	} 
      else 
	{
	  if(map->break_tab[start]!=BACKGROUND && start!=0 && 
	     map->break_tab[start-1]==map->break_tab[start])
	    {
	      return TRUE;
	    }
	  else if(map->break_tab[stop]!=BACKGROUND 
		  && stop!=map->len-1 
		  && map->break_tab[stop+1]==map->break_tab[stop])
	    {
	      return TRUE;
	    }
	}
    }
  return FALSE;
}

void
wc_wordunit_map_delete(WcWordunitMap *self)
{
  free(self->break_tab);
  free(self->assoc_tab);
  free(self);
}


void
wc_wordunit_map_init (WcWordunitMap *self,const gchar* str,gsize len)
{
  const gchar* p;
  gint offset,start;
  /* 
   * tab is equal to break tab 
   * map is equal to assoc tab
   */
  gint *tab,*assoc_tab; 
  gint marker=0;
  gsize llen;
  tab=g_new(gint,len+1);
  memset(tab,0,sizeof(gint)*(len+1));
  for(llen=len,p=str,start=0;llen>0;p++,llen--,start++)
    {
      g_assert(p!='\0');
      if ((offset=group_ad(p,llen))!=NOT_FOUND)
	marktab(tab,&marker,start,offset,len);

      if ((offset=group_ko(p,llen))!=NOT_FOUND)
	marktab(tab,&marker,start,offset,len);


      if ((offset=group_lead_vowel(p,llen))!=NOT_FOUND)
	marktab(tab,&marker,start,offset,len);


      if ((offset=group_maihun(p,llen))!=NOT_FOUND)
	marktab(tab,&marker,start,offset,len);


      if ((offset=group_tone(p,llen))!=NOT_FOUND)
	marktab(tab,&marker,start,offset,len);


      if ((offset=group_follow_vowel(p,llen))!=NOT_FOUND)
	marktab(tab,&marker,start,offset,len);


      if ((offset=group_karun(p,llen))!=NOT_FOUND)
	marktab(tab,&marker,start,offset,len);
    }  
#ifdef DEBUG  
  dump_tab(tab,len);
#endif

  assoc_tab=tab2map(tab,len);
  self->len=len;
  self->assoc_tab=assoc_tab;
  self->break_tab=tab;
}

WcWordunitMap*
wc_wordunit_map_new(const gchar* str,gsize len)
{
  WcWordunitMap *self=g_new(WcWordunitMap,1);
  wc_wordunit_map_init(self,str,len);
  return self;
}

WcWordunitMap*
wc_wordunit_find(const gchar* str,gsize len)
{
  return wc_wordunit_map_new(str,len);
}



