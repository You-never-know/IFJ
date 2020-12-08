#define DEFINE_ERR 3
#define COMPATIBLE_ERR 5
#define PARAM_ERR 6
#define RETURN_ERR 6
#define SYSTEM_ERROR 99
#define SEM_PASSED 0
#define NEW_VAR 4 
#define OTHER_SEMANTIC 7




#include "prec_table.h"
#include "symtable.h"
#include "d_tree.h"
#include "sym_list.h"




//---------------------------------------------------------------------------- prototypes
unsigned Sem_analysis(d_node * node,sym_tab * main,sym_list * list_of_tables,lex_unit_t* func_name);
bool return_not_found(lex_unit_t *name,sym_tab * main);
unsigned main_fun(sym_tab * main);