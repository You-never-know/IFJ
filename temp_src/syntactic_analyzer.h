/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
**/

#ifndef SYNTACTIC_ANALYZER
#define SYNTACTIC_ANALYZER

#include "lexical_analyzer.h"
#include "sym_list.h"
#include <stdbool.h>

bool Check_syntax(token_list * t_list, int * return_value, sym_list * id_tables, sym_tab * function_table); // make the syntactic analysis of the whole file


/******************************** syntactic functions declarations ******************************/
bool body();
bool body22();
bool body23();
bool body24();
bool body25();
bool body26();


#endif
