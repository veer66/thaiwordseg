#include <stdio.h>

typedef struct wc_pat_pos_t
{
  int start,int stop;
} WcPatPos;

#define is_thai(c) (c >= '¡' && c <= 'ì')

static void 

match_karun(const char *str,size_t len,WcPatPos *pat_pos)
{
  int i;
  
  for (i=0;i<len-3;i++)
    {
      char *p=str+i;
      if (is_thai(p[0]) && is_thai(p[1]) && is_thai(p[2])
	  
    }
}




int 
main()
{
  return 0;
}
