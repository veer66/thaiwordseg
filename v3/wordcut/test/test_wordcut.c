
#include<wordcut/wcwordcut.h>
int 
main(int argc,char **argv)
{
#define MAX_SIZE 1024
  WcWordcut wordcut;
  WC_STATUS error;
  WcWordcutResult result;
  gchar out[MAX_SIZE];
  gchar *str;
  gint n;
  wc_wordcut_init(&wordcut,&error);
  if (error==WC_RET_ERROR)
    {
      printf ("Initialize Wordcut Error.\n");
      exit(1);
    }
  str="�ҡ�������";
  n=1;
  wc_wordcut_cut(&wordcut,str,strlen(str),&result);
  printf ("Len=%d\n",wc_wordcut_result_len(&result));
  wc_wordcut_result_surface_at(&result,n,out,MAX_SIZE);
  printf ("Out = %s\n",out);
  wc_wordcut_destroy(&wordcut);
  return 0;
}