#include<wordcut/wcdict.h>
int 
main()
{
  /* ประกาศตัวแปร */
  WcDict *dict;
  WcDictIter iter;
  wc_boolean flag=WC_TRUE;
  char *str="ทางเท้าเข้าบ้าน";
  char *p;
  
  /* การสร้างวัตถุ WcDict */
  dict=wc_dict_new();

  /* การอ่านข้อมูลพจนานุกรมเข้าสู่หน่วยความจำ */
  wc_dict_load(dict,"../data/dict.etd");

  /* การกำหนดค่าตัวเลื่อนให้อยู่ที่ส่วนรากของทรัยที่ใช้
     เก็บพจนานุกรม */
  wc_dict_root(dict,&iter);
  
  /* ให้ p อยู่ที่จุดเริ่มต้นของสายอักขระ */
  p=str;

  while (flag && *p!='\0') 
    {
      /* การเลื่อนตัวเลื่อนไปในทางที่มีอักษร *p */
      wc_dict_iter_transit(&iter,*p);      
            
      /* ตรวจสอบว่ายังเลื่อนไปในทรัยได้ต่อหรือไม่ */
      flag = (iter.status==WC_DICT_ITER_ACTIVE);


      if (flag) 
	{
	  printf ("ตัวอักษร=%c\t\t\tท้ายคำ=%s\n",*p, 
		  /* ตรวจสอบว่าเป็นตัวสุดท้ายของคำหรือไม่ */
		  ( iter.terminator ? "ใช่":"ไม่ใช่" ) );
	}


      /* เลื่อน p ไปตัวถัดไป */
      p++; 
    }

  /* การทำลายวัตถุ WcDict */
  wc_dict_delete(dict);
  return 0;
}

