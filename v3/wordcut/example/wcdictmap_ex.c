#include<wordcut/wcdictmap.h>

int
main ()
{
  /* ประกาศตัวแปร */
  char *str = "โอเพ่นซอร์ส";
  WcDict dict;
  WcDictMap map;
  const WcDictIterPos *pos;
  int len = strlen (str), i, j, k, alen;

  /* การสร้างวัตถุของคลาส WcDict */
  wc_dict_init (&dict);
  /* การตรวจสอบความเรียบร้อยในการสร้าง */
  if (wc_dict_load (&dict, "../data/dict.etd") != WC_RET_NORMAL)
    {
      fprintf (stderr, "Cound not load dictionary.\n");
      exit (1);
    }

  /* การสร้างวัตถุของ WcDictMap จากคู่ของวัตถุของ WcDict และสายอักขระ */
  wc_dict_map_init (&map, &dict, str, len);
  for (i = 0; i < len; i++)
    {

      alen = wc_dict_map_assoc_len (&map, i);
      printf ("ตำแหน่งของอักษรตัวหลัง=%d,จำนวนของคำที่ลงท้ายที่ตำแหน่งนี้=%d",
	      i, alen);
      for (j = 0; j < alen; j++)
	{
	  int start;
	  /* การนำวัตถุ WcDictIterPos มาจาก WcDictMap */
	  pos = wc_dict_map_assoc_pos_at (&map, i, j);

	  /* การอ่านตำแหน่งเริ่มต้น */
	  start = wc_dict_map_assoc_at (&map, i, j);
	  for (k = 0; k < pos->len; k++)
	    {
	      wc_byte id;
	      const char *pos_str;
	      /* การใช้งาน WcDictIterPos */
	      wc_dict_iter_pos_posid (pos, k, &id);
	      pos_str = wc_dict_iter_pos_posid2str (pos, id);

	      printf (" <จุดเริ่มต้น=%d,รหัสเลข=%d,รหัสอักษร=%s> ", start, id,
		      pos_str);
	    }
	}
      printf ("\n");
    }
  /* ทำลายวัตถุของคลาส WcDictMap */
  wc_dict_map_destroy (&map);

  /* ทำลายวัตถุของคลาส WcDict */
  wc_dict_destroy (&dict);
  return 0;
}
