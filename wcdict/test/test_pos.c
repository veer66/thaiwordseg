#include<stdio.h>
#include<wordcut/wcdict.h>
#include<path.h>

int 
main() 
{ 
  WcDict *dict;
  WcDictIter iter;
  WcDictIterPos pos;
  char *str="µ“°≈¡º¡¬“«";
  const char *pos_str;
  int i=0,j,k;
  dict=wc_dict_new();
  wc_dict_load(dict,DICT_FILENAME);
  wc_dict_root(dict,&iter);
  i=0;
  while(iter.status!=WC_DICT_ITER_DEAD)
    {
      wc_dict_iter_transit(&iter,str[i]);
      if (iter.terminator)
	{
	  /* 	  printf ("%c\t%d\n",str[i],i); */
	  for(k=0;k<=i;k++)
	    {
	      printf ("%c",str[k]);
	    }
	  printf ("\n");
	  wc_dict_iter_pos(&iter,&pos);
	  for(j=0;j<pos.len;j++)
	    {
	      wc_byte id;
	      wc_dict_iter_pos_posid(&pos,j,&id);
	      pos_str=wc_dict_iter_pos_posid2str(&pos,id);
	      printf ("\tPos id = %d\tPos str=%s\n",id,pos_str);
	    }
	}
      i++;
    }
  wc_dict_delete(dict);
  return 0; 
}
