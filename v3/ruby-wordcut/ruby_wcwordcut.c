#include"ruby_wordcut.h"
#include<wordcut/wcwordcut.h>


VALUE cWordcut,cWordcutError,cWordcutResult;

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
  self=wc_wordcut_new();
  if (self==NULL)
    rb_raise(cWordcutError,"%s","Initialize wordcut error.");
  obj=Data_Wrap_Struct(klass,0,free_wc_wordcut,self);
}

static VALUE
f_wc_wordcut_cutline(VALUE obj,VALUE str)
{
  
  WcWordcut *self;

  gchar *in,*buffer;
  gint buffer_size;
  VALUE out;
  gchar *delimiter="|";
  gsize del_len=1;

  Data_Get_Struct(obj,WcWordcut,self);


  if (RSTRING(str)->len<=0)
    return rb_str_new2("");
  in=RSTRING(str)->ptr;
  buffer_size=RSTRING(str)->len*2;
  buffer=ALLOC_N(gchar,buffer_size);
  if(wc_wordcut_cutline(self,in,buffer,buffer_size,delimiter,del_len)==
     WC_RET_ERROR)     
      rb_raise(cWordcutError,"%s","Error in method cutline");    
  out=rb_str_new2(buffer);      
  free(buffer);
  return out;
}

static void
free_wc_wordcut_result(WcWordcutResult *self)
{

  if(self!=NULL)
    {
      wc_wordcut_result_destroy(self);
      free(self); 
    }

}

static VALUE
f_wc_wordcut_cut(VALUE obj,VALUE str)
{
  WcWordcut *self;
  WcWordcutResult *result;
  result=g_new(WcWordcutResult,1);
  Data_Get_Struct(obj,WcWordcut,self);
  wc_wordcut_cut(self,RSTRING(str)->ptr,RSTRING(str)->len,result);
  return Data_Wrap_Struct(cWordcutResult,0,free_wc_wordcut_result,result);  
}

static VALUE
f_wc_wordcut_result_each(VALUE obj)
{
#define MAXBUF  4096
  WcWordcutResult *self;
  gchar buf[MAXBUF];
  const WcDictIterPos *pos;
  WcDictIterPos *pos2;
  WcWordType type;

  gint i,len;
  Data_Get_Struct(obj,WcWordcutResult,self);
  len=wc_wordcut_result_len(self);
  if (len<0) 
    rb_raise(cWordcutError,"%s","Invalid wordcut result length.\n");
  
  for(i=0;i<len;i++)
    {
      /*      printf ("I=%d Len=%d\n",i,len);*/
      if(wc_wordcut_result_surface_at(self,i,buf,MAXBUF)==WC_RET_ERROR)
	{
	  rb_raise(cWordcutError,"%s","Error taking surface of word from result.");
	}
      pos=wc_wordcut_result_pos_at(self,i);
      type=wc_wordcut_result_type_at(self,i);

      rb_yield(rb_ary_new3(3,
			   rb_str_new2(buf),
			   (pos==NULL ? Qnil : 
			    Data_Wrap_Struct(ruby_wc_dict_iter_pos(),
					     0,0,(WcDictIterPos *)pos)),
			   INT2FIX(type)));	       

    }
      
  return obj;
}

void 
ruby_wcwordcut(VALUE mWordcut)
{
  cWordcutError=rb_define_class_under(mWordcut,"WordcutError",rb_eStandardError);
  cWordcut=rb_define_class_under(mWordcut,"Wordcut",rb_cObject);
  rb_define_singleton_method(cWordcut,"new",f_wc_wordcut_s_new,0);
  rb_define_method(cWordcut,"cutline",f_wc_wordcut_cutline,1);
  rb_define_method(cWordcut,"cut",f_wc_wordcut_cut,1);

  cWordcutResult=rb_define_class_under(cWordcut,"Result",rb_cObject);
  rb_define_method(cWordcutResult,"each",f_wc_wordcut_result_each,0);
  
}
