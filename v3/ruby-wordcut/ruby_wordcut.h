#ifndef __RUBY_WORDCUT_H__
#define __RUBY_WORDCUT_H__
#include<ruby.h>


#include<stdio.h>
void ruby_wcdict(VALUE mWordcut);
void ruby_wcwordunit(VALUE mWordcut);
void ruby_wcsplit(VALUE mWordcut);
void ruby_wcwordcut(VALUE mWordcut);
#endif
