#include"ruby_wordcut.h"
#include<wordcut/wcwordunit.h>

static VALUE mWcWordunit,cWcWordunitMap,eWcWordunitMap;

static void
free_wc_wordunit_map(WcWordunitMap* self)
{
  if (self!=NULL) 
      wc_wordunit_map_delete(self);
}

VALUE
f_wc_wordunit_find(VALUE obj,VALUE str)
{
  WcWordunitMap* map;
  map=wc_wordunit_find(RSTRING(str)->ptr,RSTRING(str)->len);
  return Data_Wrap_Struct(cWcWordunitMap,0,free_wc_wordunit_map,map);  
}




static VALUE
f_wc_wordunit_map_assoc(VALUE obj,VALUE stop_val)
{
  WcWordunitMap *map;
  int stop;
  int start;
  stop=FIX2INT(stop_val);
  Data_Get_Struct(obj,WcWordunitMap,map);
  start=wc_wordunit_map_assoc(map,stop);
  return (start!=WC_WORDUNIT_NULL ? INT2FIX(start) : Qnil );
}

static VALUE
f_wc_wordunit_map_break(VALUE obj,VALUE v_start,VALUE v_stop)
{
  int start=FIX2INT(v_start);
  int stop=FIX2INT(v_stop);
  WcWordunitMap *map;
  Data_Get_Struct(obj,WcWordunitMap,map);
  return (wc_wordunit_map_break(map,start,stop) ? Qtrue : Qfalse);
}

static VALUE
f_wc_wordunit_map_length(VALUE obj)
{
  WcWordunitMap *map;
  Data_Get_Struct(obj,WcWordunitMap,map);
  return INT2FIX(map->len);
}

void 
ruby_wcwordunit(VALUE mWordcut)
{  
  mWcWordunit=rb_define_module_under(mWordcut,"Wordunit");
  rb_define_module_function(mWcWordunit,"find",f_wc_wordunit_find,1);  
  cWcWordunitMap=rb_define_class_under(mWcWordunit,"Map",rb_cObject);
  rb_define_method(cWcWordunitMap,"[]",f_wc_wordunit_map_assoc,1);
  rb_define_method(cWcWordunitMap,"break",f_wc_wordunit_map_break,2);
  rb_define_method(cWcWordunitMap,"length",f_wc_wordunit_map_length,0);
}

