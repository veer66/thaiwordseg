#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wiwordcut.h"
int
main (int argc, char **argv)
{
  /*
  widict_t dict;
  char *str = "ตากลม";
  char* ans;
  WiForest forest;
  WiPath path;
  dict = load_dict ("test5.out");
  forest=str2forest (dict, str, 5);
  forest2path(forest,5,&path);
  ans=path2str(str,path,5,"|",1);
  printf ("ANS=%s\n",ans);
  free(ans);
  free (dict);*/
  WiWordcut *wordcut;
  WiAnswer* answer;
  char* str = argv[1];
  wordcut=wi_wordcut_new("test5.out");
  answer=wi_wordcut_cut(wordcut,str,strlen(str));
  printf("%s\n",wi_answer_fetch_string(answer,"|",1));
  wi_answer_delete(answer);
  wi_wordcut_delete(wordcut);
  return 0;
}
