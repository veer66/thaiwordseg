#include<ruby.h>
#include<wordcut/wcdict.h>
#include<stdio.h>

static VALUE cWcDict,rb_eWcDictError,cWcDictIter,
  cWcDictIterPos;

static void
free_wc_dict(WcDict* self)
{
  if (self!=NULL)
    {
      wc_dict_delete(self);
    }
}

static VALUE
f_wc_dict_s_new(int argc,VALUE *argv,VALUE klass)
{
  VALUE obj;
  WcDict *self;
  char *dict_filename=NULL;
  if (argc == 1)
    {
      dict_filename=RSTRING(argv[0])->ptr;
      self=wc_dict_new();
      if(wc_dict_load(self,dict_filename)!=WC_RET_NORMAL)
	{
	  rb_raise(rb_eWcDictError,"%s","Can't load encoded dictionary.");
	}
      obj=Data_Wrap_Struct(klass,0,free_wc_dict,self);
    }
  else 
    {
      rb_raise(rb_eWcDictError,"%s","Encoded dictionary filename is needed.");
    }
}



static void
mark_wc_dict_iter(WcDictIter* self)
{
  rb_gc_mark((VALUE)self->dict);
}

static void
free_wc_dict_iter(WcDictIter* self)
{
  if (self!=NULL)
    {
      free(self);
    }
}

static VALUE
f_wc_dict_root(VALUE obj)
{
  WcDict *dict;
  WcDictIter *iter;
  Data_Get_Struct(obj,WcDict,dict);
  iter=WC_NEW(WcDictIter); 
  wc_dict_root(dict,iter);
  return Data_Wrap_Struct(cWcDictIter,mark_wc_dict_iter,
			  free_wc_dict_iter,iter);
  
}

static VALUE
f_wc_dict_iter_status(VALUE obj)
{
  WcDictIter *iter;
  Data_Get_Struct(obj,WcDictIter,iter);
  return INT2FIX(iter->status);
}


static VALUE
f_wc_dict_iter_terminator(VALUE obj)
{
  WcDictIter *iter;
  Data_Get_Struct(obj,WcDictIter,iter);
  return ( iter->terminator ? Qtrue : Qfalse );
}


static VALUE
f_wc_dict_iter_transit(VALUE obj,VALUE str)
{
  WcDictIter *iter;
  if (RSTRING(str)->len != 1) 
    {
      rb_raise(rb_eWcDictError,"%s","Invalid string length for transit method.");
    }
  Data_Get_Struct(obj,WcDictIter,iter);
  wc_dict_iter_transit(iter,RSTRING(str)->ptr[0]);
  return obj;
}


static void
mark_wc_dict_iter_pos(WcDictIterPos* self)
{
  rb_gc_mark((VALUE)self->dict);
}

static void
free_wc_dict_iter_pos(WcDictIterPos* self)
{
  if(self!=NULL)
    {
      free(self);
    }
}

static VALUE
f_wc_dict_iter_pos(VALUE obj)
{
  WcDictIter *self;
  WcDictIterPos *pos;
  VALUE pos_obj;
  Data_Get_Struct(obj,WcDictIter,self);
  pos=WC_NEW(WcDictIterPos);
  wc_dict_iter_pos(self,pos);
  pos_obj=Data_Wrap_Struct(cWcDictIterPos,mark_wc_dict_iter_pos,
			   free_wc_dict_iter_pos,pos);
  return pos_obj;
}

static VALUE
f_wc_dict_iter_pos_each_id(VALUE obj)
{
  int i;
  int len;
  WcDictIterPos* self;
  Data_Get_Struct(obj,WcDictIterPos,self);
  len=self->len;
  for(i=0;i<len;i++)
    {
      wc_byte id;
      wc_dict_iter_pos_posid(self,i,&id);
      rb_yield(INT2FIX(id));
    }
  return obj;
}

static VALUE
f_wc_dict_iter_pos_each_str(VALUE obj)
{
  int i;
  int len;
  WcDictIterPos* self;
  Data_Get_Struct(obj,WcDictIterPos,self);
  len=self->len;
  for(i=0;i<len;i++)
    {
      wc_byte id;
      wc_dict_iter_pos_posid(self,i,&id);
      rb_yield(rb_str_new2(wc_dict_iter_pos_posid2str(self,id)));
    }
  return obj;
}

static VALUE
f_wc_dict_iter_pos_length(VALUE obj)
{
  WcDictIterPos* self;
  Data_Get_Struct(obj,WcDictIterPos,self);
  return INT2FIX(self->len);
  
}

void
Init_wcdict(void)
{
  rb_eWcDictError=rb_define_class("WcDictError",rb_eStandardError);  

  cWcDictIter = rb_define_class("WcDictIter",rb_cObject);
  rb_define_const(cWcDictIter,"ACTIVE",INT2FIX(WC_DICT_ITER_ACTIVE));
  rb_define_const(cWcDictIter,"DEAD",INT2FIX(WC_DICT_ITER_DEAD));
  rb_define_method(cWcDictIter,"status",f_wc_dict_iter_status,0);
  rb_define_method(cWcDictIter,"terminator",f_wc_dict_iter_terminator,0);
  rb_define_method(cWcDictIter,"transit",f_wc_dict_iter_transit,1);
  rb_define_method(cWcDictIter,"pos",f_wc_dict_iter_pos,0);
  

  cWcDict = rb_define_class("WcDict",rb_cObject);
  rb_define_singleton_method(cWcDict,"new",f_wc_dict_s_new,-1);
  rb_define_method(cWcDict,"root",f_wc_dict_root,0);
  

  cWcDictIterPos = rb_define_class("WcDictIterPos",rb_cObject);
  /*  rb_include_module(cWcDictIterPos,rb_mEnumerable); */
  rb_define_method(cWcDictIterPos,"each_id",f_wc_dict_iter_pos_each_id,0);
  rb_define_method(cWcDictIterPos,"each_str",f_wc_dict_iter_pos_each_str,0);
  rb_define_method(cWcDictIterPos,"length",f_wc_dict_iter_pos_length,0);
}
