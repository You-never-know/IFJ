/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 			xmarek72
**/

#include "create_tables.h"
#include "lexical_analyzer.h"
#include "sym_list.h"
#include "symtable.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int ERROR_SET = 0;
sym_list * result = NULL;

// set return code as the first error code
void set_error(int ERR_CODE, int *ret) {

	if (ret == NULL) {
		return;
	}

	if (ERROR_SET == 0) {
		*ret = ERR_CODE;
		ERROR_SET++;
	}
}

// clean all the alocated memory if something failed
void clean() {

	if (result == NULL) {
		return;
	}

	fprintf(stderr, "Create_table error: cleaning up\n" );

	for (sl_elem_ptr tmp = result->first; tmp != NULL; tmp = tmp->r) {

		int check = clean_table(tmp ->st_data);
		if (check != 0) {
			fprintf(stderr, "Error while freeing table\n");
			exit(99);
		}

		free_table(tmp->st_data);
	}

	sl_dissolve(result);

	exit(99);
}


/*  Create sym_tables for all the levels
 *  'file' file that is read
 *  'ret' the return value 
 *  
 *  return sym_list or empty_list if some error occured in function (malloc failed / bad parameters ...)
 */ 
sym_list * create_tables(FILE * file, int * ret) {

	// check parameter
	if (file == NULL || ret) {
		return NULL;
	}

	// set default
	*ret = 0;
	// automat states
	enum states {START, START_OF_FUNCTION, FUNCTION_ID, START_OF_PARAMETERS, END_OF_PARAMETERS, PARAMETER_ID, PARAMETER_COMMA, PARAMETER_TYPE};
	// start from the START STATE
	int state = START;


	// while condition
	bool cont = true;
	result = sl_init();
	bool keep = false;

	// remember the item that is newly created
	ht_item * remember = NULL;
	Par * par = NULL;

	while (cont) {
	
		// alloc space for lex unit
		lex_unit_t * lex = malloc(sizeof(lex_unit_t));
		if (lex == NULL) {

			fprintf(stderr, "Create_table malloc error\n");
			clean();
		}

		// get lexem
		lex = Analyze(file, lex);

		// Analyze failed or found EOF
		if (lex == NULL) {
			free(lex);
			return result;
		}


		// if any lexical error happened
		if (lex->unit_type >= 8) {
			set_error(1, ret);
			free(lex);
			continue;
		}

		// set as default
		keep = false;

		switch (state) {

			// need first function
			case START: 
						if (lex->unit_type == KEYWORD && memcmp(lex->data, "func", 4ul) == 0) {
							state = START_OF_FUNCTION;
							break;
						}
						state = START;
						break;

			case START_OF_FUNCTION:
						// if correct
						if (lex->unit_type == IDENTIFICATOR) {
							state = FUNCTION_ID; // next state

							// create new table
							sym_tab * new = htab_create(BIG_TABLE);
							if (new == NULL) {
								clean();
							}

							keep = true; // we want to keep this one
							sl_insert_last(result, new);
							sl_set_act_last(result);

							// check if defined
							ht_item * found = NULL;
							if ((found = sl_search(result, lex)) == NULL) {
								remember = add_item(new, lex, true);
								if (remember == NULL) {

								}
							}

							else {
								set_error(3, ret);
							}
							break;
						}

						// anything else
						set_error(2, ret);
						state = START_OF_FUNCTION;
						break;

			case FUNCTION_ID:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "(", 1ul) == 0) {
							state = START_OF_PARAMETERS;
							break;
						}

						set_error(2, ret);
						state = FUNCTION_ID;
						break;

			case START_OF_PARAMETERS:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ")", 1ul) == 0) {
							state = END_OF_PARAMETERS;
							break;
						}

						if (lex->unit_type == IDENTIFICATOR) {
							par = malloc_param(remember);
							if (par == NULL) {
								clean();
							}

							keep = true;
							if (add_param_name(par, lex) == false) {
								clean();
							}	
							state = PARAMETER_ID;
							break;
						}

						set_error(2, ret);
						state = START_OF_PARAMETERS;
						break;


			case PARAMETER_ID:
						if (lex->unit_type == KEYWORD) {
								int type = 0;

							if (memcmp(lex->data, "int", 3ul) == 0) {
								type = INTEGER;
							}
							else if ( memcmp(lex->data, "float64", 7ul) == 0) {
								type = DECIMAL;
							} 
							else if (memcmp(lex->data, "string", 6ul) == 0) {
								type = STRING;
							}
							else {
								set_error(2, ret);
								state = PARAMETER_ID;
								break;
							}

							state = PARAMETER_TYPE;
							if (add_param_type(par, type) == false) { // add parameter type to the parameter
								clean();
							}

							par = NULL; // set it to NULL, because we dont want to work with it anymore

							break;
						}

						set_error(2, ret);
						state = PARAMETER_ID;
						break;

			case PARAMETER_TYPE:
						// end of parameters
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ")", 1ul) == 0) {
							state = END_OF_PARAMETERS;
							break;
						}

						if (lex->unit_type == OPERATOR && memcmp(lex->data, ",", 1ul) == 0) {
							state = PARAMETER_COMMA;
							break;
						}
						set_error(2, ret);
						state = PARAMETER_TYPE;
						break;

			case PARAMETER_COMMA:
						if (lex->unit_type == IDENTIFICATOR) {
							par = malloc_param(remember);
							if (par == NULL) {
								clean();
							}

							keep = true;
							if (add_param_name(par, lex) == false) {
								clean();
							}	
							state = PARAMETER_ID;
							break;
						}

						set_error(2, ret);
						state = PARAMETER_COMMA;
						break;

			case END_OF_PARAMETERS:

						break;
			default: 
						fprintf(stderr, "Wrong state error\n");
					 	break;


		} // end of switch

		// free the unit if not needed
		if (keep == false) {
			free(lex);
		}

	} // end of while

	return result;
}