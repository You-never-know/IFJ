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

#define MEDIUM_TABLE 97 // https://planetmath.org/goodhashtableprimes 
#define BIG_TABLE 193 // https://planetmath.org/goodhashtableprimes


void set_error(int ERR_CODE, int *ret); // set RETURN_CODE as the first ERR_CODE
sym_list * create_tables(FILE * file, int * ret, sym_tab * function_table); // create linked list of sym_tables for every section
void copy_function_to_table (ht_item * copy, sym_tab *  to); // copy one function from one table to the another

#endif