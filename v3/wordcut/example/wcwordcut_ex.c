#include<wordcut/wcwordcut.h>
int
main (int argc, char **argv)
{
  /* การประกาศตัวแปร */
#define MAX_SIZE 1024
  WcWordcut wordcut;
  WcWordcutResult result;
  char out[MAX_SIZE];
  char *str;
  int i;
  WC_STATUS e;


  /* สร้าง WcWordcut */
  wc_wordcut_init (&wordcut, &e);
  if (e != WC_RET_NORMAL)
    {
      printf ("Could not initialize wordcut\n");
      exit (1);
    }
  str = "ตากลมผมยาว";

  /* สั่งตัดคำได้ผลลัพธ์เป็น WcWordcutResult */
  wc_wordcut_cut (&wordcut, str, strlen (str), &result);
  for (i = 0; i < wc_wordcut_result_len (&result); i++)
    {
      /* นำรูปของคำออกจาก WcWordcutResult */
      wc_wordcut_result_surface_at (&result, i, out, MAX_SIZE);
      printf ("%s\n", out);
    }

  /* ทำลาย WcWordcut และ WcWordcutResult */
  wc_wordcut_result_destroy (&result);
  wc_wordcut_destroy (&wordcut);
  return 0;
}
