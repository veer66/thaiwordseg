#ifndef __RUBY_WORDCUT_H__
#define __RUBY_WORDCUT_H__
#include<ruby.h>
#include<wordcut/wcdict.h>

#include<stdio.h>
void ruby_wcdict(VALUE mWordcut);
void ruby_wcwordunit(VALUE mWordcut);
void ruby_wcsplit(VALUE mWordcut);
void ruby_wcwordcut(VALUE mWordcut);
VALUE ruby_wc_dict_iter_pos();
void free_wc_dict_iter_pos(WcDictIterPos* self);
#endif
