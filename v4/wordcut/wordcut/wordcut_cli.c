/* 
 *
 * Copyright (C) 2003 Vee Satayamas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or 
 * without modification, are permitted provided that the following 
 * conditions are met:
 * 
 *   1. Redistributions of source code must retain the above 
 *      copyright notice, this list of conditions and the following 
 *      disclaimer.
 *   2. Redistributions in binary form must reproduce the above 
 *      copyright notice, this list of conditions and the following 
 *      disclaimer in the documentation and/or other materials 
 *      provided with the distribution.
 *   3. The name of the author may not be used to endorse or 
 *      promote products derived from this software without specific 
 *      prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS 
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<wordcut/wordcut.h>
#include<wordcut/xmalloc.h>
#include<wordcut/path.h>

void usage(void) {
     printf ("usage: wordcut [OPTION...]\n");
     printf ("  -D <dictionary>   specific dictionary file.\n");
     printf ("  -d <delimiter>    specific delimiter.\n");
     printf ("  -a                put aggressive delimiter.\n");
     printf ("  -h                help and exit.\n");
    
     exit(1);
}

int
main (int argc,char **argv) {
#define MAXBUF 4096
     char buffer[MAXBUF];
     Wordcut wordcut;
     WordcutResult result;
     extern char *optarg;
     extern int optind;
     int ch;
     char *delimiter="|";
     char *dictfile=WORDCUT_DATA_PATH "/tdict.wcd";
     int optD=0,optd=0;
     int aggressive=0;
    
     while((ch=getopt(argc,argv,"d:D:ha"))!= (-1)) {
	  switch(ch) {
	  case 'D':
	       dictfile=(char *)xmalloc(sizeof(char)*(strlen(optarg)+1));
	       strcpy(dictfile,optarg);
	       optD=1;
	       break;
	  case 'd':
	       delimiter=(char *)xmalloc(sizeof(char)*(strlen(optarg)+1));
	       strcpy(delimiter,optarg);
	       optd=1;
	       break;
	  case 'h':
	       usage();
	       break;
	  case 'a':
	       aggressive=1;
	       break;                                
	  default:
	       usage();
	       break;
	  }
     }
        
     if(wordcut_init(&wordcut,dictfile)!=0) {
	  fprintf (stdout,"Could not initialize wordcut.\n");
	  exit(1);
     }

     while (fgets(buffer,MAXBUF,stdin)!=NULL) {
	  char outbuf[MAXBUF];
	  char *p,*op;
	  size_t i=0;
	  int flag=0;
	  
	  p=buffer;
	  while(*p!='\0') {

	       for(op=outbuf;*p!='\0' && (*p<'¡' || *p>'ì'); p++,op++ ) {
		    *op=*p;		    
	       }	
	       if(outbuf!=op) {
		    *op='\0';
		    fputs(outbuf,stdout);
	       } 

	       /* Thai */	       
	       for(op=outbuf;*p !='\0' && *p>='¡' && *p<='ì'; p++,op++ ) {
		    *op=*p;
	       }   	       
	       if (op!=outbuf) {
		    *op='\0';
		    wordcut_cut(&wordcut,outbuf,&result);

		    if(aggressive) {
			 fputs(delimiter,stdout);
		    }

		    for(i=0;i<result.count;i++) {
			 char tword_buf[MAXBUF];
			 strncpy(tword_buf,result.str+result.start[i],result.offset[i]);
			 tword_buf[result.offset[i]]='\0';
			 fputs(tword_buf,stdout);
			 if(aggressive || i+1 != result.count) {
			      fputs(delimiter,stdout);
			 }
		    }
	       }
	       
	  }	  
     }
     
     wordcut_close(&wordcut);

     if(optd) free(delimiter);
     if(optD) free(dictfile);
     return 0;
}
