#include<wordcut/wcdict.h>
int
main ()
{
  WcDict *dict;
  WcDictIter iter;
  dict = wc_dict_new ();
  wc_dict_load (dict, "../data/dict.etd");
  wc_dict_root (dict, &iter);
  wc_dict_delete (dict);
  return 0;
}
