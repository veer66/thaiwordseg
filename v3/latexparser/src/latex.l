%{
#include<stdio.h>
#include<latexparser/latexparser.h>
#define YY_NO_UNPUT
void lxpa_symbol(gchar *txt);
void lxpa_comment(gchar *txt);   
void lxpa_env_begin(gchar *txt);    
void lxpa_env_end(gchar *txt); 	      
void lxpa_tag(gchar *txt); 		
void lxpa_tag2(gchar *txt); 		
void lxpa_block_begin(gchar *txt);  	
void lxpa_block_end(gchar *txt); 	
void lxpa_content(gchar *txt); 		
void lxpa_newline(gchar *txt); 		
void lxpa_other(gchar *txt); 		

%}
TAG 	\\[A-Za-z][A-Za-z0-9]+
%%
\%.*$   		    	{ lxpa_comment(yytext);   	}
\\[^A-Za-z0-9]              	{ lxpa_symbol(yytext); 		}
\\begin\{[a-z][a-z0-9]+\}   	{ lxpa_env_begin(yytext); 	}
\\end\{[a-z][a-z0-9]+\} 	{ lxpa_env_end(yytext); 		}
{TAG}\{                         { lxpa_tag2(yytext); 		}
{TAG}				{ lxpa_tag(yytext); 		}
\{      			{ lxpa_block_begin(yytext);  	}
\}      			{ lxpa_block_end(yytext); 	}
[0-9A-Za-z¡-ì \t]+      	{ lxpa_content(yytext); 		}
\n 				{ lxpa_newline(yytext); 		}
. 				{ lxpa_other(yytext); 		}
%%
