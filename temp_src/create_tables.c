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
#include <unistd.h>

int ERROR_SET = 0;
sym_list * result = NULL;

// clean_after all the alocated memory if something failed
void clean_after() {

	if (result == NULL) {
		return;
	}

	fprintf(stderr, "Create_table error: cleaning up\n" );

	sl_dissolve(result);

	exit(99);
}

// add implicit functions to the function table
token_list * implicit_functions(sym_tab * function_table) {

	token_list * first = NULL;
	token_list *act = malloc(sizeof(token_list));
	first = act;
	char * f = malloc(7);
	strcpy(f, "inputs");
	lex_unit_t * tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 6;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	ht_item * item = add_item(function_table, act->unit, true);
	Ret * r = malloc_ret_val(item);
	add_ret_type(r, STRING);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	//////////////////////////////////////////////////////////////////

	f = malloc(7);
	strcpy(f, "inputi");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 6;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	//////////////////////////////////////////////////////////////////
	
	f = malloc(7);
	strcpy(f, "inputf");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 6;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	r = malloc_ret_val(item);
	add_ret_type(r, DECIMAL);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	/////////////////////////////////////////////////////////////////////

	f = malloc(6);
	strcpy(f, "print");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 5;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	/////////////////////////////////////////////////////////////////////

	f = malloc(10);
	strcpy(f, "int2float");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 9;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	Par * p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "i");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, INTEGER);
	r = malloc_ret_val(item);
	add_ret_type(r, DECIMAL);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	///////////////////////////////////////////////////////////////////////

	f = malloc(10);
	strcpy(f, "float2int");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 9;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "f");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, DECIMAL);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	///////////////////////////////////////////////////////////////////////

	f = malloc(4);
	strcpy(f, "len");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 3;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "s");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, STRING);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	///////////////////////////////////////////////////////////////////////

	f = malloc(7);
	strcpy(f, "substr");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 6;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "s");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, STRING);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "i");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "n");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, INTEGER);
	r = malloc_ret_val(item);
	add_ret_type(r, STRING);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	///////////////////////////////////////////////////////////////////////

	f = malloc(4);
	strcpy(f, "ord");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 3;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "s");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, STRING);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "i");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, INTEGER);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	act->next = malloc(sizeof(token_list));
	act = act->next;

	///////////////////////////////////////////////////////////////////////

	f = malloc(4);
	strcpy(f, "chr");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 3;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;

	item = add_item(function_table, act->unit, true);
	act->next = malloc(sizeof(token_list));
	act = act->next;
	p = malloc_param(item);
	f = malloc(2);
	strcpy(f, "i");
	tf = LexUnitCreate();
	tf->data = f;
	tf->data_size = 2;
	tf->unit_type = IDENTIFICATOR;
	act->unit = tf;
	add_param_name(p, act->unit);
	add_param_type(p, INTEGER);
	r = malloc_ret_val(item);
	add_ret_type(r, STRING);
	r = malloc_ret_val(item);
	add_ret_type(r, INTEGER);
	
	return first;
}
/*
Par* malloc_param(ht_item *item); // allocates and inicializes parameter
bool add_param_name(Par* par, struct lex_unit *lex); // add data to the parameter
bool add_param_type(Par* par, int type); // add type to the parameter
Ret* malloc_ret_val(ht_item *item); // allocates and inicializes parameter
bool add_ret_type(Ret* ret, int type); // add data to the parameter
*/

// copy one item from one table to the another
void copy_function_to_table (ht_item * copy, sym_tab *  to) {

	if (copy == NULL || to == NULL) {
		return;
	}
	if (copy->func == NULL) {
		return;
	}

	ht_item *new = add_item(to, copy->func->func_name, true);
	for (Par * first = copy->func->parameters; first != NULL; first = first->next) {
		Par * p = malloc_param(new);
		add_param_name(p, first->name);
		add_param_type(p, first->type);
	}

	for (Ret * first2 = copy->func->return_val; first2 != NULL; first2 = first2->next) {
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


/*  Create sym_tables for all the levels
 *  'file' file that is read
 *  'ret' the return value 
 *  'function_table' table just for functions
 *  return sym_list or NULL if some error occured in function (malloc failed / bad parameters ...)
 *  if error happend function returns NULL and in ret the return code, function_table as NULL as well
 */ 
sym_list * create_tables(token_list * start, int * ret, sym_tab ** function_table, token_list ** to_be_cleaned) {

	// check parameter
	if (start == NULL || ret == NULL || function_table == NULL || to_be_cleaned == NULL) {
		return NULL;
	}

	if ((*function_table)== NULL) {
		*function_table = htab_create(MEDIUM_TABLE);
		if ((*function_table) == NULL) {
			return NULL;
		}
	}

	*to_be_cleaned = implicit_functions(*function_table);

	token_list * token_src = start;

	// set default as fault, because we may not hit end properly
	*ret = 2;
	// automat states
	enum states {START, START_OF_FUNCTION, FUNCTION_ID, START_OF_PARAMETERS, END_OF_PARAMETERS, PARAMETER_ID, PARAMETER_COMMA, PARAMETER_TYPE, START_OF_RETURN_TYPES, START_OF_SECTION, FOR_ERROR, RETURN,
				 END_OF_RETURN_TYPES, RETURN_TYPE, RETURN_TYPE_COMMA, ID, IF, FOR, ELSE, FUNC_NL, ADDED_TO_TABLE, ID_NL, FOR_ID_TYPE, FIRST_SEMI, SECOND_SEMI, FOR_WAIT_SEMI, FOR_DECLARATION};
	// start from the START STATE
	int state = START;

	// while condition
	bool cont = true;
	result = sl_init(); // init result -> sym_list

	// remember the item that is newly created
	ht_item * remember = NULL; // remember the item with the function
	ht_item * id_item = NULL; // remember fot the clasic table
	Par * par = NULL;
	lex_unit_t * remember_id = NULL; // remember the id
	lex_unit_t *lex = NULL;

	while (cont) {
		
		if (token_src != NULL) {
			lex = token_src->unit;
		}
		else {
			lex = NULL;
		}
		token_src = token_src->next;
		
		if (lex == NULL) { // found EOF
			if (result->first == NULL) {
				return result;
			}
			for (sl_elem_ptr ptr = result -> first; ptr != NULL; ptr = ptr -> r) {
				if ((ptr->accessible) == true) { // some table is still active
					clean_table((*function_table));
					free_table((*function_table));
					*function_table = NULL;
					sl_dissolve(result);
					return NULL;
				}
			}

			
			
			if (ERROR_SET == 0) {
				*ret = 0; // all tables are closed
			}
			else {
				clean_table((*function_table));
				free_table((*function_table));
				*function_table = NULL;
				sl_dissolve(result);
				return NULL;
			} 
			return result;
		}
		
		lex->table = NULL; // inicialize to NULL

		// if any lexical error happened
		if (lex->unit_type >= OPERATOR_ERR || lex->unit_type == ERROR) {
			set_error(1, ret);
			sl_dissolve(result);
			clean_table((*function_table));
			free_table((*function_table));
			*function_table = NULL;
			free(par);
			return NULL;
		}

		// set as default
		
		
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
							if (strcmp(lex->data, "_") == 0) {
								set_error(7, ret);
							}

							if (find_item((*function_table), lex) != NULL) {
								set_error(3, ret);
							}

							// create new table
							sym_tab * new = htab_create(BIG_TABLE);
							if (new == NULL) {
								free(par);
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
							}
							// we want to 
							sl_insert_last(result, new);
							sl_set_act_last(result);											
							remember = add_item(new, lex, true);
							if (remember == NULL) {
								clean_after();
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
							for (Par * tmp = remember->func->parameters; tmp != NULL; tmp = tmp->next) {
								if (tmp->name->data_size == lex->data_size) {
									if (strcmp(tmp->name->data, lex->data) == 0) {
										set_error(3, ret);
									}
								}
							} 
							par = malloc_param(remember);
							if (par == NULL) {
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
							}

							if (add_param_name(par, lex) == false) {
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
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

							if (lex->data_size == 3 && memcmp(lex->data, "int", 3ul) == 0) {
								type = INTEGER;
							}
							else if (lex->data_size == 7 && memcmp(lex->data, "float64", 7ul) == 0) {
								type = DECIMAL;
							} 
							else if (lex->data_size == 6 && memcmp(lex->data, "string", 6ul) == 0) {
								type = STRING;
							}
							else {
								set_error(4, ret);
								state = PARAMETER_ID;
								break;
							}
						
							ht_item * help = add_item(result->act->st_data, par->name, false); // add it to the table
							add_data(help, type); // add type to it

							state = PARAMETER_TYPE;
							if (add_param_type(par, type) == false) { // add parameter type to the parameter
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
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
							for (Par * tmp = remember->func->parameters; tmp != NULL; tmp = tmp->next) {
								if (tmp->name->data_size == lex->data_size) {
									if (strcmp(tmp->name->data, lex->data) == 0) {
										set_error(3, ret);
									}
								}
							} 
							par = malloc_param(remember);
							if (par == NULL) {
								free(par);
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
							}

							if (add_param_name(par, lex) == false) {
								free(par);	
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
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
							copy_function_to_table(remember, (*function_table)); // copy function to the function table
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

							if (lex->data_size == 3 && memcmp(lex->data, "int", 3ul) == 0) {
								type = INTEGER;
							}
							else if (lex->data_size == 7 && memcmp(lex->data, "float64", 7ul) == 0) {
								type = DECIMAL;
							} 
							else if (lex->data_size == 6 && memcmp(lex->data, "string", 6ul) == 0) {
								type = STRING;
							}
							else {
								set_error(4, ret);
								state = START_OF_RETURN_TYPES;
								break;
							}

							Ret * r = malloc_ret_val(remember);
							if (r == NULL) {
								free(par);
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
							}

							state = RETURN_TYPE;
							if (add_ret_type(r, type) == false) { // add parameter type to the parameter
								free(par);
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
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

								if (lex->data_size == 3 && memcmp(lex->data, "int", 3ul) == 0) {
									type = INTEGER;
								}
								else if (lex->data_size == 7 && memcmp(lex->data, "float64", 7ul) == 0) {
									type = DECIMAL;
								} 
								else if (lex->data_size == 6 && memcmp(lex->data, "string", 6ul) == 0) {
									type = STRING;
								}
								else {
									set_error(4, ret);
									state = START_OF_RETURN_TYPES;
									break;
								}

								Ret * r = malloc_ret_val(remember);
								if (r == NULL) {
									free(par);
									clean_table((*function_table));
									free_table((*function_table));
									*function_table = NULL;
									clean_after();
								}

								state = RETURN_TYPE;
								if (add_ret_type(r, type) == false) { // add parameter type to the parameter
									free(par);
									clean_table((*function_table));
									free_table((*function_table));
									*function_table = NULL;
									clean_after();
								}

								break;
						} // end of if

						set_error(2, ret);
						state = RETURN_TYPE_COMMA;
						break;

			case END_OF_RETURN_TYPES:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "{", 1ul) == 0) {
							copy_function_to_table(remember, (*function_table)); // copy function to the function table
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
							if (result->act != NULL) {
								sl_set_act_naccessible(result); // make the current table not accessible
								sl_set_prev_act(result); // set the previous active
								state = START_OF_SECTION;
								while (sl_get_act_accessibility(result) != true) { // set active the first accessible previous one
									if (result->act->l == NULL) { // it is the first table
										state = START;
										remember = NULL;
										break;
									}
									sl_set_prev_act(result);
								}
								break;
							}
						}
						else if (lex->unit_type == IDENTIFICATOR) {
							remember_id = lex;
														state = ID;
							break;
						}
						else if (lex->unit_type == KEYWORD && lex->data_size == 2 && memcmp(lex->data, "if", 2ul) == 0) {
							state = IF;
							break;
						} 
						else if (lex->unit_type == KEYWORD && lex->data_size == 4 && memcmp(lex->data, "else", 4ul) == 0) {
							state = IF;
							break;
						} 
						else if (lex->unit_type == KEYWORD && lex->data_size == 3 && memcmp(lex->data, "for", 3ul) == 0) {
							state = FOR;
							break;
						} 
						else if (lex->unit_type == KEYWORD && lex->data_size == 6 && memcmp(lex->data, "return", 6ul) == 0) {
							state = RETURN;
							break;
						} 
						else if (lex->unit_type == NEWLINE) {
							state = START_OF_SECTION;
							break;
						}
						set_error(2, ret);
						state = START_OF_SECTION;
						break;


			case RETURN:
						if (lex->unit_type == NEWLINE) {
							state = START_OF_SECTION;
							break;
						}
						state = RETURN;
						break;
		
			case ID:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ":=", 2ul) == 0) {
							if (strcmp(remember_id->data, "_") == 0) {
								set_error(7, ret);
							}
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
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "(", 1ul) == 0) {
							
							state = RETURN;
							break;
						}
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "=", 1ul) == 0) {
							
							state = RETURN;
							break;
						}
						
						state = START_OF_SECTION;  
						break;


			case ADDED_TO_TABLE:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "(", 1ul) == 0) {
							state = ADDED_TO_TABLE;
							break;
						}
						else if (lex->unit_type == INTEGER) {
							add_data(id_item, INTEGER);
							state = RETURN;
							break;
						}
						else if (lex->unit_type == DECIMAL) {
							add_data(id_item, DECIMAL);
							state = RETURN;
							break;
						} 
						else if (lex->unit_type == STRING) {
							add_data(id_item, STRING);
							state = RETURN;
							break;
						}
						else if (lex->unit_type == IDENTIFICATOR) {
							ht_item * found = sl_search(result, lex);
							if (found == NULL) { // not found
								set_error(4, ret);
								state = ADDED_TO_TABLE;
								break;
							} // end of if

							else { // else to the if (found == NULL)
								if (found->id->type >= NEWLINE || found->id->type <= KEYWORD) {
									set_error(4,ret);
									state = ADDED_TO_TABLE;
									break;
								}
								add_data(id_item, found->id->type);
								state = RETURN;
								break;
							} // end of else
						} // end of else if
						else {
							set_error(2, ret);
							state = ADDED_TO_TABLE;
							break;
						}

						state = ADDED_TO_TABLE;
						set_error(2, ret);
						break;


			case IF:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "{", 1ul) == 0) {
							// create new table
							sym_tab * new = htab_create(MEDIUM_TABLE);
							if (new == NULL) {
								free(par);
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
							}

							sl_insert_last(result, new);
							sl_set_act_last(result);
							state = FUNC_NL;
							break;
						}
						if (lex->unit_type == NEWLINE) {
							set_error(2, ret);
						}
						state = IF;
						break;

			case FOR:
						// create new table
						;                         
						sym_tab * new = htab_create(1); // one identificator
						if (new == NULL) {
							free(par);
							clean_table((*function_table));
							free_table((*function_table));
							*function_table = NULL;
							clean_after();
						}

						sl_insert_last(result, new);
						sl_set_act_last(result);

						if (lex->unit_type == IDENTIFICATOR) {      

							id_item = add_item(new, lex, false);
							if (id_item == NULL) {
								free(par);
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
							}
							lex->table = id_item; // add pointer where the info for this 							
														state = FOR_DECLARATION;
							break;
						}
						else if (lex->unit_type == OPERATOR && memcmp(lex->data, ";", 1ul) == 0) {
							state = FIRST_SEMI;
							break;
						}
						set_error(2, ret);
						state = FOR_ERROR; 
						break;

			case FOR_ERROR:
						if (lex->unit_type == IDENTIFICATOR) {      

							id_item = add_item(result->act->st_data, lex, false);
							if (id_item == NULL) {
								free(par);
								clean_table((*function_table));
								free_table((*function_table));
								*function_table = NULL;
								clean_after();
							}
							lex->table = id_item; // add pointer where the info for this 							
														state = FOR_DECLARATION; 
							break;
						}
						else if (lex->unit_type == OPERATOR && memcmp(lex->data, ";", 1ul) == 0) {
							state = FIRST_SEMI;
							break;
						}
						set_error(2, ret);
						state = FOR_ERROR; 
						break;

			case FOR_DECLARATION:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ":=", 2ul) == 0) {
							state = FOR_ID_TYPE;
							break;
						}
						set_error(2,ret);
						state = FOR_DECLARATION;
						break;


			case FOR_ID_TYPE:
						if (lex->unit_type == INTEGER) {
							add_data(id_item, INTEGER);
							state = FOR_WAIT_SEMI;
							break;
						}
						else if (lex->unit_type == DECIMAL) {
							add_data(id_item, DECIMAL);
							state = FOR_WAIT_SEMI;
							break;
						} 
						else if (lex->unit_type == STRING) {
							add_data(id_item, STRING);
							state = FOR_WAIT_SEMI;
							break;
						}
						else if (lex->unit_type == IDENTIFICATOR) {
							ht_item * found = sl_search(result, lex);
							if (found == NULL) { // not found
								set_error(4, ret);
								state = FOR_WAIT_SEMI;
								break;
							} // end of if

							else { // else to the if (found == NULL) -> we found it
								if ((found->id->type >= NEWLINE || found->id->type <= KEYWORD) ) {
									set_error(4,ret);
									state = FOR_WAIT_SEMI;
									break;
								}
								add_data(id_item, found->id->type);
								state = FOR_WAIT_SEMI;
								break;
							} // end of else
						} // end of else if
						else {
							set_error(4, ret);
							state = FOR_ID_TYPE;
							break;
						}

						set_error(2, ret);
						state = FOR_ID_TYPE;
						break;

			case FOR_WAIT_SEMI:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ";", 1ul) == 0) {
							state = FIRST_SEMI;
							break;
						}

						state = FOR_WAIT_SEMI;
						break;

			case FIRST_SEMI:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, ";", 1ul) == 0) {
							sl_set_act_naccessible(result);
							sl_set_prev_act(result);
							state = SECOND_SEMI;
							break;
						}
						state = FIRST_SEMI;
						break;

			case SECOND_SEMI:
						if (lex->unit_type == OPERATOR && memcmp(lex->data, "{", 1ul) == 0) {
								// create new table                         
								sym_tab * new = htab_create(MEDIUM_TABLE); // medium table
								if (new == NULL) {
									free(par);
									clean_table((*function_table));
									free_table((*function_table));
									*function_table = NULL;
									clean_after();
								}
								sl_insert_last(result, new);
								sl_set_act_last(result);
								state = FUNC_NL;
								break;
						}

						state = SECOND_SEMI;
						break;

					
			default: 
						fprintf(stderr, "Wrong state error\n");
					 	break;

		} // end of switch
	} // end of while 
	

	return result;
}
