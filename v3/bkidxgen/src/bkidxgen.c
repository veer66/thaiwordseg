#include"bigrams.h"
#include"join.h"
#include<wordcut/wcsplit.h>
#include<wordcut/wcwordcut.h>
#include<latexparser/latexparser.h>

gint 
main(gint argc,gchar** argv)
{
  BiGrams bi;
  WC_STATUS error;
  WcWordcut wordcut;
  Join join;
  if (argc!=2)
    {
      g_error("Invalid argument");
    }
  bi_grams_init(&bi,3);
  wc_wordcut_init(&wordcut,&error);
  if (error==WC_RET_ERROR)
    {
      g_error("Init wordcut error.");
    }
  bi_grams_latex(argv[1],&bi,&wordcut);
  
  // bi_grams_dump(&bi);
  join_init(&join);
  join_file(&join,argv[1],&wordcut,&bi);
  join_destroy(&join);
  bi_grams_destroy(&bi);
  wc_wordcut_destroy(&wordcut);
  return 0;
}
