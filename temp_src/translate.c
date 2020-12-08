#include "create_tables.h"
#include "structs.h"
#include "symtable.h"
#include "lexical_analyzer.h"
#include "syntactic_analyzer.h"
#include "sym_list.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>

void clean_token_list(token_list * t_list) {

	if (t_list == NULL) {
		return;
	}

	for (token_list * tmp = t_list; tmp != NULL;) {
		token_list * to_be_deleted = tmp;
		tmp = tmp->next;
		//LexUnitDelete(to_be_deleted->unit);
		free(to_be_deleted);
	}
}


int main() {

	int return_value = 0;
	FILE * f = stdin;
	sym_tab * function_table = NULL;
	token_list * file_tokens = Loading_lex_units(f);
	token_list * implicit_functions = NULL;
	sym_list *file_tables = create_tables(file_tokens, &return_value, &function_table, &implicit_functions);

	if (return_value != 0) {
		clean_token_list(file_tokens);
		//clean_token_list(implicit_functions);
		return return_value;
	}

	return_value = Check_syntax(file_tokens, file_tables, function_table);

	// clean memory
	if (file_tables != NULL) {
		sl_dissolve(file_tables);
	}
	if (function_table != NULL) {
		clean_table(function_table);
		free_table(function_table);
	}
	clean_token_list(file_tokens);
	//clean_token_list(implicit_functions);

	return return_value;
	
}

