#include<wordcut/wcdict.h>
int
main ()
{
  /* ประกาศตัวแปร */
  WcDict *dict;
  WcDictIter iter;
  WcDictIterPos iter_pos;
  wc_byte id;
  wc_boolean flag = WC_TRUE;
  int i;
  char *str = "ทางเท้าเข้าบ้าน";
  char *p;
  const char *pos;

  /* การสร้างวัตถุ WcDict */
  dict = wc_dict_new ();

  /* การอ่านข้อมูลพจนานุกรมเข้าสู่หน่วยความจำ */
  wc_dict_load (dict, "../data/dict.etd");

  /* การกำหนดค่าตัวเลื่อนให้อยู่ที่ส่วนรากของทรัยที่ใช้
     เก็บพจนานุกรม */
  wc_dict_root (dict, &iter);

  /* ให้ p อยู่ที่จุดเริ่มต้นของสายอักขระ */
  p = str;

  while (flag && *p != '\0')
    {
      /* การเลื่อนตัวเลื่อนไปในทางที่มีอักษร *p */
      wc_dict_iter_transit (&iter, *p);

      /* ตรวจสอบว่ายังเลื่อนไปในทรัยได้ต่อหรือไม่ */
      flag = (iter.status == WC_DICT_ITER_ACTIVE);


      if (flag)
	{
	  printf ("ตัวอักษร=%c\t\tท้ายคำ=%s\t", *p,
		  /* ตรวจสอบว่าเป็นตัวสุดท้ายของคำหรือไม่ */
		  (iter.terminator ? "ใช่" : "ไม่ใช่"));
	  if (iter.terminator)
	    {
	      printf ("ชนิดของคำ\t");
	      /* การเรียกใช้วัตถุ WcDictIterPos */
	      wc_dict_iter_pos (&iter, &iter_pos);

	      /* การเลื่อนไปบน POS แต่ละตัว */
	      for (i = 0; i < iter_pos.len; i++)
		{
		  /* การนำออก POS ID */
		  wc_dict_iter_pos_posid (&iter_pos, i, &id);

		  /* การเปลี่ยน POS ID เป็นสายอักขระ */
		  pos = wc_dict_iter_pos_posid2str (&iter_pos, id);

		  printf ("%d,%s ", id, pos);
		}
	    }
	  printf ("\n");
	}
      /* เลื่อน p ไปตัวถัดไป */
      p++;
    }

  /* การทำลายวัตถุ WcDict */
  wc_dict_delete (dict);
  return 0;
}
