/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
* Drengubiak Vladimír	        xdreng01
**/

#ifndef SYNTACTIC_ANALYZER
#define SYNTACTIC_ANALYZER

#include "lexical_analyzer.h"
#include "symtable.h"
#include "sym_list.h"
#include "d_tree.h"
#include <stdbool.h>

int Check_syntax(token_list * t_list, sym_list * id_tables, sym_tab * function_table); // make the syntactic analysis of the whole file


/******************************** syntactic functions declarations ******************************/
bool NL5(lex_unit_t*);
bool NL4(lex_unit_t*);
bool type(lex_unit_t*);
bool par_list(lex_unit_t*);
bool par_list_start(lex_unit_t*);
bool params(lex_unit_t*);
bool ret_list(lex_unit_t*);
bool ret_list_start(lex_unit_t*);
bool ret_vals(lex_unit_t*);
bool expression(lex_unit_t* act, d_node * root);
bool body(lex_unit_t*);
bool body22(lex_unit_t*);
bool body23(lex_unit_t*);
bool body24(lex_unit_t*);
bool body25(lex_unit_t*);
bool body26(lex_unit_t*);
bool id_list(lex_unit_t* act, d_node * root);
bool id_choose(lex_unit_t*);
bool id_choose29(lex_unit_t* act, d_node * assignment); 
bool id_choose30(lex_unit_t* act, d_node * assignment);
bool id_choose31(lex_unit_t*);
bool else_r(lex_unit_t* act, d_node * closing_bracket);
bool definition(lex_unit_t* act, d_node * root);
bool assignment_38_39(lex_unit_t* act, d_node * root);
bool exp_fun(lex_unit_t* act, d_node * id);
bool next(lex_unit_t* act, d_node * id);
bool exp_list(lex_unit_t* act, d_node * id);
bool exp_list_start(lex_unit_t* act, d_node * body);
bool fun2(lex_unit_t*);
bool prog();

#endif
