#include<wordcut/wcwordcut.h>
int 
main(int argc,char **argv)
{
#define MAX_SIZE 1024
  WcWordcut wordcut;
  WcWordcutResult result;
  char out[MAX_SIZE];
  char *str;
  int n,i;
  WcDict dict;
  if (wc_dict_load(&dict,"../data/dict.etd")!=WC_RET_NORMAL)
    {
      fprintf(stderr,"Cound not load dictionary.\n");
      exit(1);
    }

  wc_wordcut_init_with_dict(&wordcut,&dict);

  str="µ“°≈¡º¡¬“«";
  n=1;

  wc_wordcut_cut(&wordcut,str,strlen(str),&result);
  for (i=0;i<wc_wordcut_result_len(&result);i++)
    {
      wc_wordcut_result_surface_at(&result,i,out,MAX_SIZE);
      printf ("Output=%s\n",out);
    }
  wc_wordcut_result_destroy(&result);
  wc_wordcut_destroy(&wordcut);
  wc_dict_destroy(&dict);
  return 0;
}
