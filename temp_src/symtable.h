#ifndef SYM_TABLE
#define SYM_TABLE


#include "structs.h"
#include <string.h>     // size_t
#include <stdbool.h>    // bool

// parameters of the function
typedef struct param {
	lex_unit_t * name; // name of the identificator
	int type; // type of the parameter
	
	struct param * next; // pointer to the next parameter - linked list
} Par;

// return values of the function
typedef struct ret_val {
	int type; // return type
		
	struct ret_val * next; // pointer to the next return value - linked list
} Ret;

// item in the # table == symtable
typedef struct htable_item {

	lex_unit_t * name; // name of the identificator
	lex_unit_t * data; // value of the identificator, active only for id not for fun
	bool is_function; // sets functions and identificators aside
	
	// if it is function
	 
	Par * parameters; // linked list of parameters with their types
	Ret * return_val; // linked list of return values of the function

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
sym_tab *htab_init(size_t n); // mallocs memmory and initalizates it
ht_item *add_item(lex_unit_t *lex, sym_tab *st, bool is_function); // add identificator to the htable
ht_item *find_item(lex_unit_t * lex, sym_tab *st); // find lexem in the table NULL if it not there 


#endif 