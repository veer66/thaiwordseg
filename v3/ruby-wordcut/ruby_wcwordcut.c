#include"ruby_wordcut.h"
#include<wordcut/wcwordcut.h>


VALUE cWordcut,cWordcutError;

static void
free_wc_wordcut (WcWordcut* self)
{
  if(self!=NULL) wc_wordcut_delete(self);
}

static VALUE
f_wc_wordcut_s_new(VALUE klass)
{
  VALUE obj;
  WcWordcut *self;
  WC_STATUS error;
  self=wc_wordcut_new();
  if (self==NULL)
    rb_raise(cWordcutError,"%s","Initialize wordcut error.");
  obj=Data_Wrap_Struct(klass,0,free_wc_wordcut,self);
  return obj;
}

static VALUE
f_wc_wordcut_cutline(VALUE obj,VALUE str)
{
  WcWordcut *self;
  self
}

void 
ruby_wcwordcut(VALUE mWordcut)
{
  cWordcutError=rb_define_class_under(mWordcut,"WordcutError",rb_eStandardError);
  cWordcut=rb_define_class_under(mWordcut,"Wordcut",rb_cObject);
  rb_define_method(cWordcut,"new",f_wc_wordcut_s_new,0);
}
