#ifndef SYM_TABLE
#define SYM_TABLE


#include "structs.h"
#include <string.h>     // size_t
#include <stdbool.h>    // bool

// parameters of the function
typedef struct param {
	lex_unit_t * name; // name of  the id
	int type; // type of the parameter
	
	struct param * next; // pointer to the next parameter
} Par;


typedef struct ret_val {
	int type; // return type
		
	struct ret_val * next; // pointer to the next return value
} Ret;

typedef struct htable_item {

	lex_unit_t * name; // name of the identificator
	lex_unit_t * data; // value of the identificator, active only for id not for fun
	bool is_function; // sets functions and identificators aside
	
	// if it is function
	 
	Par * parameters; // linked list of parameters with their types
	Ret * return_val; // linked list of return values of the function

	struct htable_item * next; // pointer to the next item
} ht_item;	

// table of symbols
typedef struct hash_table { 	
	size_t size; // number of items
	size_t arr_size; // size of the array of pointers
	ht_item *ptr[]; // array of pointers to items	
} sym_tab;

//----------------------------------------------------------------------------
size_t htab_hash_fun(const char* str); // hash function 
sym_tab *htab_init(size_t n); // htable creator
ht_item *add_id(lex_unit_t *lex, sym_tab *st, bool is_function); // add identificator to the htable


#endif 