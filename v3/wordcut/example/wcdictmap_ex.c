#include<wordcut/wcdictmap.h>

int 
main()
{
  /* ��С�ȵ���� */
  char *str="��ҵҡ��";
  WcDict dict;
  WcDictMap map;
  const WcDictIterPos *pos;
  int len=strlen(str),i,j,k,alen;

  /* ������ҧ�ѵ�آͧ���� WcDict */
  wc_dict_init(&dict);
  /* ��õ�Ǩ�ͺ�������º����㹡�����ҧ */
  if (wc_dict_load(&dict,"../data/dict.etd")!=WC_RET_NORMAL)
    {
      fprintf(stderr,"Cound not load dictionary.\n");
      exit(1);
    }

  /* ������ҧ�ѵ�آͧ WcDictMap �ҡ���ͧ�ѵ�آͧ WcDict �������ѡ��� */
  wc_dict_map_init(&map,&dict,str,len);
  for(i=0;i<len;i++) 
    {
      alen=wc_dict_map_assoc_len(&map,i);
      printf ("���˹觢ͧ�ѡ�õ����ѧ=%d,�ӹǹ�ͧ�ӷ��ŧ���·����˹觹��=%d",i,alen);
      for(j=0;j<alen;j++) 
	{
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
  /* ������ѵ�آͧ���� WcDictMap */
  wc_dict_map_destroy(&map);

  /* ������ѵ�آͧ���� WcDict */
  wc_dict_destroy(&dict);
  return 0;
}
