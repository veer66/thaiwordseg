#include<wordcut/wcwordunit.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int
main ()
{
  /* การประกาศตัวแปร */
  char *str = "โอเพ่นซอร์ส";
  size_t str_len;
  WcWordunitMap *wu_map;
  int i;

  str_len = strlen (str);
  /* การค้นหาสายอักขระย่อยที่ต้องตามรูปแบบของคำ */
  wu_map = wc_wordunit_find (str, str_len);
  printf ("คำ = %s\n", str);
  for (i = 0; i < str_len; i++)
    {
      int start;
      /* อ่านค่าตำแหน่งเริ่มต้นของสายอักขระที่ต้องตามรูปแบบของคำ
         ที่มีอักขระตัวสุดท้ายที่ตำแหน่งที่ i */
      start = wc_wordunit_map_assoc (wu_map, i);
      if (start != WC_WORDUNIT_NULL)
	printf ("เริ่มต้น=%d\tสิ้นสุด=%d\n", start, i);
    }
  return 0;
}
