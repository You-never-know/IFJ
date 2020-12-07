/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 			xmarek72
**/

#ifndef CREATE_TABLE
#define CREATE_TABLE

#include "symtable.h"
#include "structs.h"
#include "sym_list.h"
#include "lexical_analyzer.h"

#define MEDIUM_TABLE 11 // https://planetmath.org/goodhashtableprimes 
#define BIG_TABLE 23 // https://planetmath.org/goodhashtableprimes


void set_error(int ERR_CODE, int *ret); // set RETURN_CODE as the first ERR_CODE
sym_list * create_tables(token_list * start, int * ret, sym_tab ** function_table, token_list ** to_be_cleaned); // create linked list of sym_tables for every section
void copy_function_to_table (ht_item * copy, sym_tab *  to); // copy one function from one table to the another
token_list * implicit_functions(sym_tab * function_table); // add implicit functions to the function list

#endif