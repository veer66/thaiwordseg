#include<wordcut/wcdict.h>
int
main ()
{
  /* ��С�ȵ���� */
  WcDict *dict;
  WcDictIter iter;
  WcDictIterPos iter_pos;
  wc_byte id;
  wc_boolean flag = WC_TRUE;
  int i;
  char *str = "�ҧ�����Һ�ҹ";
  char *p;
  const char *pos;

  /* ������ҧ�ѵ�� WcDict */
  dict = wc_dict_new ();

  /* �����ҹ�����ž��ҹء��������˹��¤����� */
  wc_dict_load (dict, "../data/dict.etd");

  /* ��á�˹���ҵ������͹�����������ǹ�ҡ�ͧ���·����
     �纾��ҹء�� */
  wc_dict_root (dict, &iter);

  /* ��� p ������ش������鹢ͧ����ѡ��� */
  p = str;

  while (flag && *p != '\0')
    {
      /* �������͹�������͹�㹷ҧ������ѡ�� *p */
      wc_dict_iter_transit (&iter, *p);

      /* ��Ǩ�ͺ����ѧ����͹�㹷�������������� */
      flag = (iter.status == WC_DICT_ITER_ACTIVE);


      if (flag)
	{
	  printf ("����ѡ��=%c\t\t���¤�=%s\t", *p,
		  /* ��Ǩ�ͺ����繵���ش���¢ͧ��������� */
		  (iter.terminator ? "��" : "�����"));
	  if (iter.terminator)
	    {
	      printf ("��Դ�ͧ��\t");
	      /* ������¡���ѵ�� WcDictIterPos */
	      wc_dict_iter_pos (&iter, &iter_pos);

	      /* �������͹仺� POS ���е�� */
	      for (i = 0; i < iter_pos.len; i++)
		{
		  /* ��ù��͡ POS ID */
		  wc_dict_iter_pos_posid (&iter_pos, i, &id);

		  /* �������¹ POS ID ������ѡ��� */
		  pos = wc_dict_iter_pos_posid2str (&iter_pos, id);

		  printf ("%d,%s ", id, pos);
		}
	    }
	  printf ("\n");
	}
      /* ����͹ p 仵�ǶѴ� */
      p++;
    }

  /* ��÷�����ѵ�� WcDict */
  wc_dict_delete (dict);
  return 0;
}
