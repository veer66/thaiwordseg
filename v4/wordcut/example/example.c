/* Wordcut using example. */

#include <stdio.h>
#include <wordcut/wordcut.h>

int 
main () 
{
    Wordcut wordcut;
    WordcutResult result;
    char *pch;
    char buffer[4096];
    int i;
    if(wordcut_init(&wordcut,"../data/tdict.wcd")!=0) {
    	printf("Could not initialize wordcut\n");
	exit(1);
    }
    wordcut_cut(&wordcut,"╪абрг",&result);

    for(i=0;i<result.count;i++) {
        strncpy(buffer,
                result.str+result.start[i],
                result.offset[i]);
        buffer[result.offset[i]]='\0';
        printf ("%s\t",buffer);
    }
    printf ("\n");
         
    wordcut_result_close(&result);
    wordcut_close(&wordcut);
    return 0;
}

