#include<stdio.h>
#include<stdlib.h>
#include<wordcut/wordcut_list.h>
#include<wordcut/xmalloc.h>

typedef enum 
{
    LEX_UNIT_CONTROL=1,
    LEX_UNIT_CONTENT
} LexUnitType;

typedef struct 
{
    unsigned char ch;
    LexUnitType   lex_type;
} LexUnit;

static WordcutList*  /* list of LexUnit */
re_lex_analyze(const unsigned char *rule_str)
{
    size_t i;
    size_t len;
    int    slash = 0;
    WordcutList *list;
    list = (WordcutList *)xmalloc(sizeof(WordcutList));
    len = strlen(rule_str);
    wordcut_list_init(list,len);
    for(i=0; i<len; i++) {
        if(slash) {
            LexUnit *lexunit  = (LexUnit *)xmalloc(sizeof(LexUnit));
            lexunit->lex_type = LEX_UNIT_CONTENT;
            lexunit->ch       = rule_str[i];
            wordcut_list_append(list, lexunit);
            slash             = 0;
        } else {
            if(rule_str[i]=='\\') {
                slash = 1;
            } else if(rule_str[i]=='[' ||
                      rule_str[i]==']' ||
                      rule_str[i]=='*' ||
                      rule_str[i]==')' ||
                      rule_str[i]=='(' ||
                      rule_str[i]=='+' ||
                      rule_str[i]=='?' ||
                      rule_str[i]=='-' ||
                      rule_str[i]=='.' ) 
            {
                LexUnit *lexunit   = (LexUnit *)xmalloc(sizeof(LexUnit));
                lexunit->lex_type  = LEX_UNIT_CONTROL;
                lexunit->ch        = rule_str[i];
                wordcut_list_append(list, lexunit);
            } else {
                LexUnit *lexunit  = (LexUnit *)xmalloc(sizeof(LexUnit));
                lexunit->lex_type = LEX_UNIT_CONTENT;
                lexunit->ch       = rule_str[i];
                wordcut_list_append(list, lexunit);
            }
        }
    }
    return list;
}

static void
re_lex_dump(WordcutList *list)
{
    size_t i;
    for(i=0;i<list->size;++i) {
        printf ("%c %s\n",((LexUnit*)wordcut_list_get(list,i))->ch,
                          ((LexUnit*)wordcut_list_get(list,i))->lex_type == LEX_UNIT_CONTROL ?  "control" : "content"
               );
    }
}

static void 
re_lex_free(WordcutList *list)
{
    size_t i;
    for(i=0;i<list->size;++i) {
        free((LexUnit*)wordcut_list_get(list,i));
    }
    wordcut_list_destroy(list);
}
		
int 
main (int argc,char** argv)
{
    char *input_filename=NULL;
    FILE *input_file;
    if (argc != 3) {
        fprintf (stderr,
            "Usage: wordcut_rule_prepare <regular expression rule file> <output DFA file>\n");
        exit(1);
    }
    input_filename = argv[1];
    input_file = fopen(input_filename, "r");
    if(input_file == NULL) {
        fprintf(stderr,"Cannot open RE rule file ``%s''\n",input_filename);
        exit(1);
    }
    fclose(input_file);
    return 0;
}
