#include<ruby.h>
#include<wordcut/wcdict.h>
#include<stdio.h>

static VALUE cWcThaiTokenizerError,mWcThaiTokenizer;

#define DEFAULT_TOK  1
#define THAI_TOK     2

static void 
callback(const char *str,size_t len,void* data)
{
  int lang=(int)data;
  rb_yield(rb_assoc_new(rb_str_new(str,len),INT2FIX(lang)));
}

static VALUE
f_wc_thai_tokenizer_tok(VALUE obj,VALUE arg)
{
  if (wc_thai_tokenizer(RSTRING(arg)->ptr,callback,(void *)THAI_TOK,
			callback,(void *)DEFAULT_TOK)!=0)
    {
      rb_raise(cWcThaiTokenizerError,"%s","wc_thai_tokenizer returned error flag.");
    }
  return obj;
}


void
Init_wcthaitokenizer(void)
{
  cWcThaiTokenizerError=rb_define_class("WcThaiTokenizerError",rb_eStandardError);  
  mWcThaiTokenizer=rb_define_module("WcThaiTokenizer");
  rb_define_module_function(mWcThaiTokenizer,"tok",f_wc_thai_tokenizer_tok,1);
  rb_define_const(mWcThaiTokenizer,"Thai",INT2FIX(THAI_TOK));
  rb_define_const(mWcThaiTokenizer,"Default",INT2FIX(DEFAULT_TOK));
  
}
