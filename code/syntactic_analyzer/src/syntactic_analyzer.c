/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
*
**/

#include "structs.h"
#include "symtable.h"
#include "sym_list.h"
#include "lexical_analyzer.h"
#include "d_tree.h"
#include <stdbool.h>
#include "lex_list.h"
#include "prec_parser.h"

token_list * Active_token = NULL;


lex_unit_t * getNextToken() {

	if (Active_token == NULL) {
		return NULL;
	}

	lex_unit_t * to_be_returned = Active_token->unit;
	Active_token = Active_token->next;
	return to_be_returned;
}

lex_unit_t * getActiveToken() {

	if (Active_token == NULL) {
		return NULL;
	}

	return Active_token->unit;
}

/*****************************************************************  Syntactic functions part **********************************************************************/








/*****************************************************************  End of syntactic function part **********************************************************************/

// Main function

bool Check_syntax(token_list * t_list, int * return_value, sym_list * id_tables, sym_tab * function_table) {

	if (t_list == NULL || return_value == NULL || id_tables == NULL || function_table == NULL) {
		if (return_value != NULL) {
			*return_value = 99;
		}
		return false;
	}


	return true;

} 