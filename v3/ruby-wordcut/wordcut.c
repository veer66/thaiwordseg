#include"ruby_wordcut.h"

void
Init_wordcut(void)
{
  VALUE mWordcut;
  mWordcut=rb_define_module("Wordcut");
  ruby_wcdict(mWordcut);
  ruby_wcwordunit(mWordcut);
  ruby_wcsplit(mWordcut);
  ruby_wcwordcut(mWordcut);
}
