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
    char buffer[MAXBUF],thbuf[MAXBUF],abuf[MAXBUF],wbuf[MAXBUF];
    char *bf,*p,*tp;
    int state=0,i;
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

    bf=abuf;

    state=(-1);
    
    while (fgets(buffer,MAXBUF,stdin)!=NULL) {
        for(p=buffer;*p!='\0';p++) {
            if(*p>='¡' && *p<='ì') {
                
                if(state==0) {
                    *bf='\0';

                    if(aggressive) {
                        fputs(delimiter,stdout);
                    }
                    fputs(abuf,stdout);
                    
                    bf=thbuf;
                    *bf=*p;
                    bf++;
                    state=1;
                } else if (state==(-1)) {
                    bf=thbuf;
                    *bf=*p;
                    bf++;
                    state=1;
                } else {
                    *bf =*p;
                    bf++;
                }
            } else {

                if(state!=0) {
                    
                    *bf='\0';
                    wordcut_cut(&wordcut,thbuf,&result);

                    if(aggressive) {
                        fputs(delimiter,stdout);
                    }
                    
                    
                    for(i=0;i<result.count;i++) {
                        int j;
                        char *op;
                        for(j=0,tp=result.str+result.start[i],op=wbuf;
                            j<result.offset[i];j++,op++,tp++) {
                            
                            *op=*tp;
                     
                        }
                        *op='\0';
                        fputs(wbuf,stdout);
                        if(i+1!=result.count)
                            fputs(delimiter,stdout);
                    }
                    
                    state=0;
                    bf=abuf;
                    *bf=*p;
                    bf++;
                } else if (state==(-1)) {
                    bf=abuf;
                    *bf =*p;
                    bf++;                
                } else {
                    /* state0 */
                    *bf =*p;
                    bf++;
                }
            }
        }

    }

    if(*bf>='¡' && *bf<='ì') {
        if (state==1) {
            *bf='\0';
            if(aggressive) {
                fputs(delimiter,stdout);
            }

            fputs(abuf,stdout);
        }
    } else if (state==0) {
        *bf='\0';
        if(aggressive) {
            fputs(delimiter,stdout);
        }

        fputs(abuf,stdout);
    }

    wordcut_close(&wordcut);

    if(optd) free(delimiter);
    if(optD) free(dictfile);
    return 0;
}
