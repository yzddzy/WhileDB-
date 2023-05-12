#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"
#include "L1.h"
#include "lib.h"
#include "lexer.h"
#include "parser.h"
extern struct cmd* root;
extern struct SU_hash_table* hashtable;
struct L1_basic_blocks *bbs;
int yyparse();
int main(int argc, char **argv){
    hashtable =init_SU_hash();
    yyin=stdin;
    yyparse();
    fclose(stdin);
    struct L1_cmd_listbox *res=TAC_gen(root);
    print_L1_cmd_listbox(res);
    printf("%s\n","--------------------------------------------------------");
    print_L1_basic_blocks(BB_gen(res));


   
}