#include"latex.h"

lxpa_callback m_symbol_callback;
gpointer m_symbol_data;
lxpa_callback m_comment_callback;
gpointer m_comment_data;
lxpa_callback m_env_begin_callback;
gpointer m_env_begin_data;
lxpa_callback m_env_end_callback;
gpointer m_env_end_data;
lxpa_callback m_tag_callback;
gpointer m_tag_data;
lxpa_callback m_tag2_callback;
gpointer m_tag2_data;
lxpa_callback m_block_begin_callback;
gpointer m_block_begin_data;
lxpa_callback m_block_end_callback;
gpointer m_block_end_data;
lxpa_callback m_content_callback;
gpointer m_content_data;
lxpa_callback m_newline_callback;
gpointer m_newline_data;
lxpa_callback m_other_callback;
gpointer m_other_data;

void
lxpa_symbol(char *txt)
{
  if (m_symbol_callback!=NULL)
    m_symbol_callback(txt,m_symbol_data);
}

void
lxpa_init()
{
}

void
lxpa_input(const gchar* filename)
{
  FILE *fp;
  fp=fopen(filename,"r");
  if(fp==NULL)
    g_error("Can't open file %s.",filename);
  
  yyin=fp;
}

void 
lxpa_destroy(char *txt)
{
}


void 
lxpa_comment(char *txt)
{
  if (m_comment_callback!=NULL)
    m_comment_callback(txt,m_comment_data);
}

void 
lxpa_env_begin(char *txt)
{
  if (m_env_begin_callback!=NULL)
    m_env_begin_callback(txt,m_env_begin_data);
}

void 
lxpa_env_end(char *txt)
{
  if (m_env_end_callback!=NULL)
    m_env_end_callback(txt,m_env_end_data);
}

void 
lxpa_tag(char *txt)
{
  if (m_tag_callback!=NULL)
    m_tag_callback(txt,m_tag_data);
}

void 
lxpa_tag2(char *txt)
{
  if (m_tag2_callback!=NULL)
    m_tag2_callback(txt,m_tag2_data);
}

void 
lxpa_block_begin(char *txt)
{
  if (m_block_begin_callback!=NULL)
    m_block_begin_callback(txt,m_block_begin_data);  
}

void 
lxpa_block_end(char *txt)
{
  if (m_block_end_callback!=NULL)
    m_block_end_callback(txt,m_block_end_data);  
}

void 
lxpa_content(char *txt)
{
  if (m_content_callback!=NULL)
    m_content_callback(txt,m_content_data);  
}

void 
lxpa_newline(char *txt)
{
  if (m_newline_callback!=NULL)
      m_newline_callback(txt,m_newline_data);  
}

void 
lxpa_other(char *txt)
{
  if (m_other_callback!=NULL)
    m_other_callback(txt,m_other_data);  
}

void 
lxpa_set_callback(lxpa_callback symbol_callback,gpointer symbol_data,
		  lxpa_callback comment_callback,gpointer comment_data,
		  lxpa_callback env_begin_callback,gpointer env_begin_data,
		  lxpa_callback env_end_callback,gpointer env_end_data,
		  lxpa_callback tag_callback,gpointer tag_data,
		  lxpa_callback tag2_callback,gpointer tag2_data,
		  lxpa_callback block_begin_callback,gpointer block_begin_data,
		  lxpa_callback block_end_callback,gpointer block_end_data,
		  lxpa_callback content_callback,gpointer content_data,
		  lxpa_callback newline_callback,gpointer newline_data,
		  lxpa_callback other_callback,gpointer other_data)
{
  m_symbol_callback          = symbol_callback;
  m_symbol_data              = symbol_data;
  m_comment_callback         = comment_callback;
  m_comment_data             = comment_data;
  m_env_begin_callback       = env_begin_callback;
  m_env_begin_data           = env_begin_data;
  m_env_end_callback         = env_end_callback;
  m_env_end_data             = env_end_data;
  m_tag_callback             = tag_callback;
  m_tag_data                 = tag_data;
  m_tag2_callback            = tag2_callback;
  m_tag2_data                = tag2_data;
  m_block_begin_callback     = block_begin_callback;
  m_block_begin_data         = block_begin_data;
  m_block_end_callback       = block_end_callback;
  m_block_end_data           = block_end_data;
  m_content_callback         = content_callback;
  m_content_data             = content_data;
  m_newline_callback         = newline_callback;
  m_newline_data             = newline_data;
  m_other_callback           = other_callback;
  m_other_data               = other_data;
}

void
lxpa_parse()
{
  yylex();
}
