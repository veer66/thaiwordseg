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
  wc_boolean is_break;
  int start, stop;

  str_len = strlen (str);
  /* ��ä�������ѡ������·���ͧ����ٻẺ�ͧ�� */
  wu_map = wc_wordunit_find (str, str_len);

  start = 0;
  stop = 1;
  /* ��Ǩ�ͺ��Ҥ���ѹ�Ѻ (�ش�������,�ش����ش) ������Դ��âѴ�ٻẺ�ͧ��������� */
  is_break = wc_wordunit_map_break (wu_map, start, stop);
  printf ("�������=%d,����ش=%d,�Ѵ=%s\n", start, stop,
	  is_break ? "��" : "���");

  start = 0;
  stop = 2;
  /* ��Ǩ�ͺ��Ҥ���ѹ�Ѻ (�ش�������,�ش����ش) ������Դ��âѴ�ٻẺ�ͧ��������� */
  is_break = wc_wordunit_map_break (wu_map, start, stop);
  printf ("�������=%d,����ش=%d,�Ѵ=%s\n", start, stop,
	  is_break ? "��" : "���");

  return 0;
}
