#include<wordcut/wcsplit.h>
#include<stdio.h>
#include<stdlib.h>

int
wc_split(const char *source,
	     wc_split_callback_t thai_callback,
	     void* thai_support_data,
	     wc_split_callback_t default_callback,
	     void* default_support_data)
{
  int flag=1;
  int i=0;
  while (flag)
    {
      int j;
      int dot_count;
      /* seeking for thai */
      j=i;
      dot_count=0;
      while(source[j] != '\0' 
	    && ((source[j] >= '¡' && source[j] <= 'î')|| source[j]=='.')) 
	{
	  j++;
	  if (source[j]=='.') dot_count++;
	}
      if (source[j] == '\0') flag=0;
      if (j!=i) 
	{
	  size_t len;
	  char *buffer;
	  int k,p;
	  /* default */	      
	  len=(j-i);
	  buffer=malloc((len+1)*sizeof(char));
	  if (buffer == NULL) return 1;
	  for(k=i,p=0;k<j;k++,p++) buffer[p]=source[k];
	  buffer[p]='\0';
	  if (len==dot_count) 
	    {
              if(default_callback!=NULL)
		default_callback(buffer,len,default_support_data);
	    }
	  else
	    {
              if(thai_callback!=NULL)
	        thai_callback(buffer,len,thai_support_data);
	    }
	  free(buffer);
	}
      i=j;
      while(source[j] != '\0' && !(source[j] >= '¡' && source[j] <= 'î'))
	{
	  j++;
	}
      if (source[j] == '\0') flag=0;
      if (j!=i)
	{
	  /* default */
	  size_t len;
	  char *buffer;
	  int k,p;
	  len=(j-i);
	  buffer=(char*)malloc(sizeof(char)*(len+1));
	  if (buffer==NULL) return 1;
	  
	  for(p=0,k=i,p=0;k<j;k++,p++) buffer[p]=source[k];
	  buffer[p]='\0';
	  
          if(default_callback!=NULL)
	    default_callback(buffer,len,default_support_data);
	  free(buffer);
	}
      i=j;
    } 
  return 0;
}

/*
int 
main(int argc,char **argv)
{
  wc_thai_english_tokenizer("µÒ¡ÅÁ abcd",thai_dump,NULL,english_dump,NULL);
  return 0;
}
*/






