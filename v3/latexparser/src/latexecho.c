#include<latexparser/latexparser.h>
#include<stdio.h>

static gpointer 
lxpa_echo(gchar *txt,gpointer data)
{
  gchar *type=(gchar*)data;
  printf ("Type=%s : Content=%s\n",type,txt);
  return NULL;
}



int 
main()
{
  lxpa_init();
  lxpa_set_callback(lxpa_echo,"Symbol",
		    lxpa_echo,"Comment",
		    lxpa_echo,"EnvBegin",
		    lxpa_echo,"EnvEnd",
		    lxpa_echo,"Tag",
		    lxpa_echo,"Tag2",
		    lxpa_echo,"BlockBegin",
		    lxpa_echo,"BlockEnd",
		    lxpa_echo,"Content",
		    lxpa_echo,"NewLine",
		    lxpa_echo,"Other");
  lxpa_parse();
  lxpa_destroy();
  return 0;
}
