/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
**/

#ifndef PREC_PARSER
#define PREC_PARSER

#include "structs.h"
#include "d_tree.h"
#include <stdbool.h>

void clean_stack(lex_list * list); // free data from the stack
bool Parse_expresion(lex_unit_t ** token, d_node * root, FILE * f, sym_tab * fun_tab); // make the precedence analysis of the expresion


#endif
