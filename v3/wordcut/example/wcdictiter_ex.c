#include<wordcut/wcdict.h>
int 
main()
{
  /* ��С�ȵ���� */
  WcDict *dict;
  WcDictIter iter;
  wc_boolean flag=WC_TRUE;
  char *str="�ҧ�����Һ�ҹ";
  char *p;
  
  /* ������ҧ�ѵ�� WcDict */
  dict=wc_dict_new();

  /* �����ҹ�����ž��ҹء��������˹��¤����� */
  wc_dict_load(dict,"../data/dict.etd");

  /* ��á�˹���ҵ������͹�����������ǹ�ҡ�ͧ���·����
     �纾��ҹء�� */
  wc_dict_root(dict,&iter);
  
  /* ��� p ������ش������鹢ͧ����ѡ��� */
  p=str;

  while (flag && *p!='\0') 
    {
      /* �������͹�������͹�㹷ҧ������ѡ�� *p */
      wc_dict_iter_transit(&iter,*p);      
            
      /* ��Ǩ�ͺ����ѧ����͹�㹷�������������� */
      flag = (iter.status==WC_DICT_ITER_ACTIVE);


      if (flag) 
	{
	  printf ("����ѡ��=%c\t\t\t���¤�=%s\n",*p, 
		  /* ��Ǩ�ͺ����繵���ش���¢ͧ��������� */
		  ( iter.terminator ? "��":"�����" ) );
	}


      /* ����͹ p 仵�ǶѴ� */
      p++; 
    }

  /* ��÷�����ѵ�� WcDict */
  wc_dict_delete(dict);
  return 0;
}

