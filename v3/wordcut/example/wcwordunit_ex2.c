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
  wc_boolean is_break;
  int start, stop;

  str_len = strlen (str);
  /* การค้นหาสายอักขระย่อยที่ต้องตามรูปแบบของคำ */
  wu_map = wc_wordunit_find (str, str_len);

  start = 0;
  stop = 1;
  /* ตรวจสอบว่าคู่อันดับ (จุดเริ่มต้น,จุดสิ้นสุด) ทำให้เกิดการขัดรูปแบบของคำหรือไม่ */
  is_break = wc_wordunit_map_break (wu_map, start, stop);
  printf ("เริ่มต้น=%d,สิ่นสุด=%d,ขัด=%s\n", start, stop,
	  is_break ? "ใช่" : "ไม่");

  start = 0;
  stop = 2;
  /* ตรวจสอบว่าคู่อันดับ (จุดเริ่มต้น,จุดสิ้นสุด) ทำให้เกิดการขัดรูปแบบของคำหรือไม่ */
  is_break = wc_wordunit_map_break (wu_map, start, stop);
  printf ("เริ่มต้น=%d,สิ่นสุด=%d,ขัด=%s\n", start, stop,
	  is_break ? "ใช่" : "ไม่");

  return 0;
}
