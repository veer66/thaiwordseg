#include<wordcut/wcwordcut.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


static void 
usage()
{
  printf ("Usage : wordcut \n"
	  "\n"
	  "Command line interface for wordcut\n");
}

int 
main(int argc,char** argv)
{
#define   IN_SIZE     4096
#define   OUT_SIZE    8192
  FILE *fin,*fout;
  char bin[IN_SIZE],bout[OUT_SIZE];
  char delimiter[]="|";
  int del_len;
  WcWordcut wordcut;
  WC_STATUS error;

  if (argc==2 && ( strcasecmp(argv[1],"-h")==0 || strcasecmp(argv[1],"--help")==0))
    {
      usage();
      exit(0);
    }


  fin=stdin;
  fout=stdout;
  wc_wordcut_init(&wordcut,&error);
  if(error==WC_RET_ERROR)
    WC_HALT("Initialize wordcut error.");

  del_len=strlen(delimiter);
  while(fgets(bin,IN_SIZE,fin)!=NULL)
    {
      wc_wordcut_cutline(&wordcut,bin,bout,OUT_SIZE,delimiter,del_len);
      fputs(bout,fout);
    }
  wc_wordcut_destroy(&wordcut);
  return 0;
}
