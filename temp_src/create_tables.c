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

// copy one item from one table to the another
void copy_function_to_table (ht_item * copy, sym_tab *  to) {

	if (copy == NULL || to == NULL) {
		return;
	}

	if (copy->is_function == false) {
		return;
	}

	ht_item *new = add_item(to, copy->name, true);
	for (Par * first = copy->parameters; first != NULL; first = first->next) {
		Par * p = malloc_param(new);
		add_param_name(p, first->name);
		add_param_type(p, first->type);
	}

	for (Ret * first2 = copy->return_val; first2 != NULL; first2 = first2->next) {
		Ret * r = malloc_ret_val(new);
		add_ret_type(r, first2->type);
	}

}


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
 *  'function_table' table just for functions
 *  return sym_list or empty_list if some error occured in function (malloc failed / bad parameters ...)
 */ 
sym_list * create_tables(FILE * file, int * ret, sym_tab * function_table) {

	// check parameter
	if (file == NULL || ret) {
		return NULL;
	}

	// set default as fault, because we may not hit end properly
	*ret = 2;
	// automat states
	enum states {START, START_OF_FUNCTION, FUNCTION_ID, START_OF_PARAMETERS, END_OF_PARAMETERS, PARAMETER_ID, PARAMETER_COMMA, PARAMETER_TYPE, START_OF_RETURN_TYPES, START_OF_SECTION,
				 END_OF_RETURN_TYPES, RETURN_TYPE, RETURN_TYPE_COMMA, ID, IF, FOR, ELSE, IF_DECIDE, FUNC_NL, SECTION_NL, END_OF_SECTION, ADDED_TO_TABLE, ID_NL};
	// start from the START STATE
	int state = START;

	// while condition
	bool cont = true;
	result = sl_init(); // init result
	bool keep = false;

	// remember the item that is newly created
	ht_item * remember = NULL; // remember the item with the function
	ht_item * id_item = NULL; // remember fot the clasic table
	Par * par = NULL;
	lex_unit_t * remember_id = NULL; // remember the id

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
			// check if all tables were closed
			if (result->first == NULL) {
				return result;
			}
			for (sl_elem_ptr ptr = result -> first; ptr != NULL; ptr = ptr -> r) {
				if ((ptr->accessible) == true) { // some table is still active
					clean();
					free(remember_id);
					free(par);
					return result;
				}
			}
			*ret = 0; // all tables are closed
			return result;
		}

		lex->table = NULL; // inicialize to NULL

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


							remember = add_item(new, lex, true);
							if (remember == NULL) {
								clean();
							}
							lex->table = remember; // add pointer where the info for this 
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
							for (Par * tmp = remember->parameters; tmp != NULL; tmp = tmp->next) {
								if (sizeof(tmp->name->data) == sizeof(lex->data)) {
									if (strcmp(tmp->name->data, lex->data) == 0) {
										set_error(3, ret);
									}
								}
							} 
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
								set_error(4, ret);
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

						else if (lex->unit_type == OPERATOR && memcmp(lex->data, ",", 1ul) == 0) {
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
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "(", 1ul) == 0) {
							state = START_OF_RETURN_TYPES;
							break;
						}
						else if (lex->unit_type == OPERATOR && memcmp(lex->data, "{", 1ul) == 0) {
							state = FUNC_NL;
							break;
						}

						set_error(2, ret);
						state = END_OF_PARAMETERS;
						break;

			case START_OF_RETURN_TYPES:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ")", 1ul) == 0) {
							state = END_OF_RETURN_TYPES;
							break;
						}
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
								set_error(4, ret);
								state = START_OF_RETURN_TYPES;
								break;
							}

							Ret * r = malloc_ret_val(remember);
							if (r == NULL) {
								clean();
							}

							state = RETURN_TYPE;
							if (add_ret_type(r, type) == false) { // add parameter type to the parameter
								clean();
							}

							break;
						} // end of if

						set_error(2, ret);
						state = START_OF_RETURN_TYPES;
						break;

			case RETURN_TYPE:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ")", 1ul) == 0) {
							state = END_OF_RETURN_TYPES;
							break;
						}
						else if (lex->unit_type == OPERATOR && memcmp(lex->data, ",", 1ul) == 0) {
							state = RETURN_TYPE_COMMA;
							break;
						}

						set_error(2, ret);
						state = RETURN_TYPE;
						break;

			case RETURN_TYPE_COMMA:
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
									set_error(4, ret);
									state = START_OF_RETURN_TYPES;
									break;
								}

								Ret * r = malloc_ret_val(remember);
								if (r == NULL) {
									clean();
								}

								state = RETURN_TYPE;
								if (add_ret_type(r, type) == false) { // add parameter type to the parameter
									clean();
								}

								break;
						} // end of if

						set_error(2, ret);
						state = RETURN_TYPE_COMMA;
						break;

			case END_OF_RETURN_TYPES:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "{", 1ul) == 0) {
							copy_function_to_table(remember, function_table); // copy function to the function table
							state = FUNC_NL;
							break;
						}

						set_error(2, ret);
						state = END_OF_RETURN_TYPES;
						break;

			case FUNC_NL:
						if (lex->unit_type == NEWLINE) {
							state = START_OF_SECTION;
							break;
						}
						set_error(2, ret);
						state = FUNC_NL;
						break;

			case START_OF_SECTION:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "}", 1ul) == 0) {
							state = SECTION_NL;
							break;
						}
						else if (lex->unit_type == IDENTIFICATOR) {
							remember_id = lex;
							keep = true;
							state = ID;
							break;
						}
						else if (lex->unit_type == KEYWORD && memcmp(lex->data, "if", 2ul) == 0) {
							state = IF;
							break;
						} 
						else if (lex->unit_type == KEYWORD && memcmp(lex->data, "for", 3ul) == 0) {
							state = FOR;
							break;
						} 
						else if (lex->unit_type == NEWLINE) {
							state = START_OF_SECTION;
							break;
						}
						state = START_OF_SECTION;
						break;

			
			case SECTION_NL:
						if (lex->unit_type == NEWLINE) {
							state = END_OF_SECTION;
							break;
						}
						set_error(2, ret);
						state = SECTION_NL;
						break;

			case END_OF_SECTION:
						if (result->act != NULL) {
							sl_set_act_naccesible(result); // make the current table not accesible
							sl_set_prev_act(result); // set the previous active
							while (sl_get_act_accesibility(result) != true) { // set active the first accesible previous one
								if (result->act->l == NULL) { // it is the first table
									state = START;
									break;
								}
								sl_set_prev_act(result);
							}
							state = START_OF_SECTION;
							break;
						}
						set_error(2, ret);
						state = END_OF_SECTION;
						break;

			case ID:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ":=", 2ul) == 0) {
							if (find_item(result->act->st_data, remember_id) == NULL) {
								ht_item * item = add_item(result->act->st_data ,remember_id ,false);
								id_item = item; // remember this item
								remember_id->table = item;
								state = ADDED_TO_TABLE;
								break;
							}
							else {
								set_error(3,ret);  
							}
						}
						state = START_OF_SECTION;  
						break;


			case ADDED_TO_TABLE:
						if (lex->unit_type == KEYWORD) {
							if (memcmp(lex->data, "int", 3ul) == 0) {
								add_data(id_item, lex);
								keep = true;
								state = ID_NL;
								break;
							}
							else if (memcmp(lex->data, "float64", 7ul) == 0) {
								add_data(id_item, lex);
								keep = true;
								state = ID_NL;
								break;
							} 
							else if (memcmp(lex->data, "string", 6ul) == 0) {
								add_data(id_item, lex);
								keep = true;
								state = ID_NL;
								break;
							}
							else {
								set_error(4, ret);
								state = ADDED_TO_TABLE;
								break;
							}
						}
						if (lex->unit_type == IDENTIFICATOR) {
							ht_item * found = sl_search(result, lex);
							if (found == NULL) { // not found
								for (Par * tmp = remember->parameters; tmp != NULL; tmp = tmp->next) {
									
									if (sizeof(tmp->name->data) == sizeof(lex->data)) { // compare amount of memory
										if (strcmp(tmp->name->data, lex->data) == 0) { // compare if the memory is same 
											keep = true;
											lex->unit_type = tmp->type; // set the type as the type of the parameter
											add_data(id_item, lex);
											break;
										} // end of if
									} // end of if
								} // end of for

								if (keep == false ){
									set_error(4, ret);
								}
								break;
							} // end of if

							else {
								if (found->data == NULL) {
									set_error(4,ret);
									break;
								}
								add_data(id_item, found->data);
							} // end of else
						} // end of if

						state = ADDED_TO_TABLE;
						set_error(2, ret);
						break;

			case ID_NL:
						if (lex->unit_type == NEWLINE) {
							state = START_OF_SECTION;
							break;
						}
						state = SECTION_NL;
						break;


			case IF:
						// create new table
						// check \n
						break;


			case IF_DECIDE:
						// decide if else or start of section
						break;

			case ELSE:
						// create new table
						// check \n
						break;


			case FOR:
						// create new table
						// check \n
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