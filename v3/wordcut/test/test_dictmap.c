#include<wordcut/wcdictmap.h>
#include<path.h>
int 
main()
{
  char *str="ปลาตากลม";
  WcDict dict;
  WcDictMap map;
  const WcDictIterPos *pos;
  int len=strlen(str),i,j,k,alen;

  wc_dict_init(&dict);
  printf ("!!!\n");
  if (wc_dict_load(&dict,"../data/dict.etd")!=WC_RET_NORMAL)
    {
      fprintf(stderr,"Cound not load dictionary.\n");
      exit(1);
    }

  wc_dict_map_init(&map,&dict,str,len);
  printf ("%s\n",str);
  for(i=0;i<len;i++) 
    {
      alen=wc_dict_map_assoc_len(&map,i);

      printf ("i=%d\tlen=%d",i,alen);

      for(j=0;j<alen;j++) 
	{
	  printf ("\t%d",wc_dict_map_assoc_at(&map,i,j));
	  pos=wc_dict_map_assoc_pos_at(&map,i,j);

	  for(k=0;k<pos->len;k++) 
	    {
	      wc_byte id;
	      const char *pos_str;
	      wc_dict_iter_pos_posid(pos,k,&id);
	      pos_str=wc_dict_iter_pos_posid2str(pos,id);
	      printf ("<%d,%s>",id,pos_str);
	    }
	}
      printf ("\n");
    }
  wc_dict_map_destroy(&map);
  wc_dict_destroy(&dict);
  return 0;
}
