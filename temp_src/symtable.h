/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 			xmarek72
**/

#ifndef SYM_TABLE
#define SYM_TABLE


#include "structs.h"    // struct lex_unit
#include <string.h>     // size_t
#include <stdbool.h>    // bool

// parameters of the function
typedef struct param {
	struct lex_unit * name; // name of the identificator
	int type; // type of the parameter
	
	struct param * next; // pointer to the next parameter - linked list
} Par;

// return values of the function
typedef struct ret_val {
	int type; // return type
		
	struct ret_val * next; // pointer to the next return value - linked list
} Ret;

// item in the # table == symtable
typedef struct function {
	struct lex_unit * func_name; // name of the function
	Par * parameters; // linked list of parameters with their types
	Ret * return_val; // linked list of return values of the function

} Func;

typedef struct identificator {
	struct lex_unit * id_name; // name of the identificator
	int type; // type of the identificator
	bool accesible; // was declared already
} Id;

typedef struct htable_item {	
	Id * id; // struct of identificator
	Func * func; // struct of function

	struct htable_item * next; // pointer to the next item
} ht_item;	

// table of symbols == hash table
typedef struct hash_table { 	
	size_t size; // number of items
	size_t arr_size; // size of the array of pointers
	ht_item *ptr[]; // array of pointers to items	
} sym_tab;

//----------------------------------------------------------------------------
size_t htab_hash_fun(const char* str); // hash function from 'http://www.cse.yorku.ca/~oz/hash.html'
sym_tab *htab_create(size_t n); // mallocs memmory and initalizates it
ht_item *add_item(sym_tab *st, struct lex_unit *lex, bool is_function); // add identificator to the htable
ht_item *find_item(sym_tab *st, struct lex_unit * lex); // find lexem in the table NULL if it not there 
bool add_data(ht_item *item, int type); // add data to indentificator
Par* malloc_param(ht_item *item); // allocates and inicializes parameter
bool add_param_name(Par* par, struct lex_unit *lex); // add data to the parameter
bool add_param_type(Par* par, int type); // add type to the parameter
Ret* malloc_ret_val(ht_item *item); // allocates and inicializes parameter
bool add_ret_type(Ret* ret, int type); // add data to the parameter
int clean_table(sym_tab *st); // removes all items from the table
void free_table(sym_tab *st); // free the given table from memory
bool add_access(ht_item * item,bool access); //add access to ID by default is false
bool is_function(ht_item * item); // find if the item is a function

#endif 
