#include "ruby_wordcut.h"
#include <wordcut/wcsplit.h>

static VALUE cWcSplitError,mWcSplit;

#define DEFAULT_TOK  1
#define THAI_TOK     2

static void 
callback(const char *str,size_t len,void* data)
{
  int lang=(int)data;
  rb_yield(rb_assoc_new(rb_str_new(str,len),INT2FIX(lang)));
}

static VALUE
f_wc_split(VALUE obj,VALUE arg)
{
  if (wc_split(RSTRING(arg)->ptr,callback,(void *)THAI_TOK,
			callback,(void *)DEFAULT_TOK)!=0)
    {
      rb_raise(cWcSplitError,"%s","wc_thai_tokenizer returned error flag.");
    }
  return obj;
}

void
ruby_wcsplit(VALUE mWordcut)
{
  mWcSplit=rb_define_module_under(mWordcut,"Split");

  cWcSplitError=rb_define_class_under(mWcSplit,"SplitError",rb_eStandardError);  

  rb_define_module_function(mWcSplit,"split",f_wc_split,1);

  rb_define_const(mWcSplit,"Thai",INT2FIX(THAI_TOK));
  rb_define_const(mWcSplit,"Default",INT2FIX(DEFAULT_TOK));

}
