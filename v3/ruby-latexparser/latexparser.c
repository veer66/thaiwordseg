#include<ruby.h>
#include<latexparser/latexparser.h>

static VALUE mLatexParser;


#define LP_SYMBOL             1
#define LP_COMMENT            2
#define LP_ENV_BEGIN          3
#define LP_ENV_END            4
#define LP_TAG                5
#define LP_TAG2               6
#define LP_BLOCK_BEGIN        7
#define LP_BLOCK_END          8
#define LP_CONTENT            9
#define LP_NEWLINE            10
#define LP_OTHER              11


static void*
callback(char *txt,void* data)
{
  int type=(int)data;
  rb_yield(rb_assoc_new(rb_str_new2(txt),INT2FIX(type)));  
}

static VALUE
f_latex_parse(int argc,VALUE *argv,VALUE obj)
{
  lxpa_init();
  if (argc>=1)
    {
      lxpa_input(RSTRING(argv[0])->ptr);
    }
  lxpa_set_callback(callback,(void *)LP_SYMBOL,
		    callback,(void *)LP_COMMENT,            
		    callback,(void *)LP_ENV_BEGIN,          
		    callback,(void *)LP_ENV_END,            
		    callback,(void *)LP_TAG,                
		    callback,(void *)LP_TAG2,               
		    callback,(void *)LP_BLOCK_BEGIN,        
		    callback,(void *)LP_BLOCK_END,          
		    callback,(void *)LP_CONTENT,            
		    callback,(void *)LP_NEWLINE,            
		    callback,(void *)LP_OTHER);	    
  lxpa_parse();
  lxpa_destroy();
}

void
Init_latexparser (void)
{
  mLatexParser=rb_define_module("LatexParser");
  rb_define_const(mLatexParser,"SYMBOL",INT2FIX(LP_SYMBOL));
  rb_define_const(mLatexParser,"COMMENT",INT2FIX(LP_COMMENT));            
  rb_define_const(mLatexParser,"ENV_BEGIN",INT2FIX(LP_ENV_BEGIN));          
  rb_define_const(mLatexParser,"ENV_END",INT2FIX(LP_ENV_END));            
  rb_define_const(mLatexParser,"TAG",INT2FIX(LP_TAG));                
  rb_define_const(mLatexParser,"TAG2",INT2FIX(LP_TAG2));               
  rb_define_const(mLatexParser,"BLOCK_BEGIN",INT2FIX(LP_BLOCK_BEGIN));        
  rb_define_const(mLatexParser,"BLOCK_END",INT2FIX(LP_BLOCK_END));          
  rb_define_const(mLatexParser,"CONTENT",INT2FIX(LP_CONTENT));            
  rb_define_const(mLatexParser,"NEWLINE",INT2FIX(LP_NEWLINE));            
  rb_define_const(mLatexParser,"OTHER",INT2FIX(LP_OTHER));              
  rb_define_module_function(mLatexParser,"parse",f_latex_parse,-1);
}

