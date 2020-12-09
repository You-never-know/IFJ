/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
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
#include "lexical_analyzer.h"

bool Parse_expresion(lex_unit_t * token, d_node * root, token_list ** start, sym_tab * fun_tab); // make the precedence analysis of the expresion


#endif
