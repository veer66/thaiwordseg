
#include<wordcut/wcwordunit.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int
main ()
{
  /* ��û�С�ȵ���� */
  char *str = "���蹫����";
  size_t str_len;
  WcWordunitMap *wu_map;
  int i,j;

  str_len = strlen (str);
  /* ��ä�������ѡ������·���ͧ����ٻẺ�ͧ�� */
  wu_map = wc_wordunit_find (str, str_len);
  printf ("�� = %s\n", str);
  for (i = 0; i < str_len; i++)
    {
      int start;
      /* ��ҹ��ҵ��˹�������鹢ͧ����ѡ��з���ͧ����ٻẺ�ͧ��
         ������ѡ��е���ش���·����˹觷�� i */
      start = wc_wordunit_map_assoc (wu_map, i);
      if (start != WC_WORDUNIT_NULL)
	printf ("�������=%d\t����ش=%d\n", start, i);
    }
  for (i=0;i<str_len;i++)
    {
      for(j=i;j<str_len;j++)
	{
	  printf ("�������=%d\t����ش=%d\t�Դ��=%s\n", i, j,
		  wc_wordunit_map_break(wu_map,i,j) ? "�Դ" : "���");
	}
    }
  return 0;
}
