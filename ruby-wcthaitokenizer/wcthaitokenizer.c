#include<ruby.h>
#include<wordcut/wcdict.h>
#include<stdio.h>

static VALUE cWcThaiTokenizerError;

void
Init_wcthaitokenizer(void)
{
  cWcThaiTokenizerError=rb_define_class("WcThaiTokenizerError",rb_eStandardError);  
}
