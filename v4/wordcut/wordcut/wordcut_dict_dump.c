#include<wordcut/wordcut_dict.h>
#include<stdio.h>

static char dump_buffer[1000];

static void dump_(uint8_t *data,int p,int c) {
    uint8_t header=data[p],size;
    int i;


    printf ("!!! p=%d header=%d size=%d\n",p,header,size); 
        
    if (header & 0x1) {
        /* complete */

        int i;
        for (i=0;i<c;i++) {
            printf ("%c",dump_buffer[i]);
        }
        printf ("\n");
    }
    size=data[p+1];

                                
    if (size > 0) {
        if (header & 0x2) {
            /* suffix compress */
            for (i=0;i<size;i++) {

                dump_buffer[c+i]=(char)data[p+2+i];
            }
            dump_(data,data[p+2+size] | (data[p+3+size]<<8) | (data[p+4+size]<<16),c+size);
        } else {
            for(i=0;i<size;i++) {
                dump_buffer[c]=data[p+2+4*i];
                dump_(data,data[p+3+4*i] | (data[p+4+4*i]<<8) | (data[p+5+4*i]<<16) ,c+1);
            }
        }
    }
}

void
wordcut_dict_dump(WordcutDict *dict)
{
    dump_(dict->data,1,0);
}


int
main (int argc,char** argv) {
    WordcutDict dict;
    WordcutDictNode node;
    int x;
    if (argc != 2) {
        fprintf (stderr,"usage: wordcut_dict_dump <dictfile>\n");
        exit(1);
    }
    if (wordcut_dict_init(&dict,argv[1])!=0) {
        fprintf (stderr,"Could not init dict.\n");
        exit(1);
    }

    wordcut_dict_dump(&dict);

    wordcut_dict_close(&dict);
    
    return 0;
}
