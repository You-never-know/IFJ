/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 			xmarek72
**/


#include "symtable.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


// hash function from http://www.cse.yorku.ca/~oz/hash.html
size_t htab_hash_fun(const char* str) {

	uint32_t h=0;     // must have 32 bits

    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}

// malloc space for new hash table and inicialize it 
// return pointer to new symtable or NULL if allocation failed
sym_tab *htab_init(size_t n) {

	if (n == 0) {
		return NULL;
	}

	sym_tab * new = malloc(sizeof (sym_tab)  + n*sizeof(ht_item *)); // malloc space for table and all of the pointers to item

	if (new == NULL) {
		fprintf(stderr, "Symtable malloc error\n");
		return NULL;
	}


	new->arr_size = n;
	new->size = 0;

	for (size_t i = 0; i<n; i++) { // initialize pointers

		new->ptr[i] = NULL;

	}

	return new;

}


// help function for adding - find previous item
// return last on the index or NULL if no item exists on the index
ht_item *find_place(lex_unit_t *lex, sym_tab *st) {

	if (st == NULL || lex == NULL) {
		fprintf(stderr, "find_place failed\n");
		return NULL;
	}


	size_t idx = htab_hash_fun((const char*)lex->data) % st->arr_size;

	// find last item
	ht_item * found = st->ptr[idx];
	for (ht_item *tmp = found; tmp != NULL; tmp = tmp->next) {
		found = tmp;
	} 

	return found;
}

// help function for adding
// adds already allocated item to the start index of table
void add_item_to_the_start(sym_tab *st, lex_unit_t *lex, ht_item* new) {

	if (st == NULL || lex == NULL || new == NULL) {
		fprintf(stderr, "find_place failed\n");
	}

	size_t idx = htab_hash_fun((const char*)lex->data) % st->arr_size;

	st->ptr[idx] = new;
}


// malloc space and add identificator to the htable
// return pointer to the new unit or NULL if malloc failed or NULL given in parameters
ht_item *add_item(lex_unit_t *lex, sym_tab *st, bool is_function) {

	if (st == NULL || lex == NULL) {
		fprintf(stderr, "Add_id failed\n");
		return NULL;
	}

	ht_item * new = malloc(sizeof(ht_item));
	if (new == NULL) {
		fprintf(stderr, "Symtable item malloc error\n");
		return NULL;
	}

	st->size = st->size + 1;
	new->name = lex;

	new->is_function = is_function;
	new->data = NULL;

	new->parameters = NULL;
	new->return_val = NULL;
	new->next = NULL;

	ht_item* place = find_place(lex, st);

	if (place == NULL) {
		add_item_to_the_start(st, lex, new);
		return new;
	}

	place->next = new;

	return new;

}

// find item in the table
// return item if found, NULL if not
ht_item* find_item(lex_unit_t * lex, sym_tab *st) {

	if (lex == NULL || st == NULL) {
		fprintf(stderr, "find item error\n");
		return NULL;
	}

	size_t idx = htab_hash_fun((const char*)lex->data) % st->arr_size;


	for (ht_item *tmp = st->ptr[idx]; tmp!= NULL; tmp = tmp->next) {

		if (tmp->data == lex) {
			return tmp;
		}

	}

	return NULL;
} 


// add data to the given identificator
// if it is a function or it doesn't exit return false
// else return true
bool add_data(ht_item *item, lex_unit_t * lex) {

	if (item == NULL || lex == NULL) {
		fprintf(stderr, "add_data error\n");
		return false;
	}

	if (item->is_function == true) {
		item->data = lex;
		return true;
	}

	return false;
}

// allocates and inicializes parameter and adds it to the linked list
// return pointer to Par if success, NULL if not
Par* malloc_param(ht_item *item) {

	if (item == NULL) {
		fprintf(stderr, "malloc_param failed\n");
		return NULL;
	}

	Par * parameter = malloc(sizeof(Par));

	if (parameter == NULL) {
		fprintf(stderr, "Malloc parameter failed\n");
		return NULL;
	}

	parameter->next = NULL;
	parameter->name = NULL;
	parameter->type = 0;

	// first parameter
	if (item->parameters == NULL) {
		item->parameters = parameter;
	}

	// there are already some
	else {

		// find last parameter, add it there
		Par * found = item->parameters;
		for (Par * tmp = found; tmp != NULL; tmp = tmp->next) {
			found = tmp;
		}

		found->next = parameter;
	}

	return parameter;

}


// add data to the parameter
// return true if success, false if not
bool add_param_data(Par* par, lex_unit_t *lex) {

	if (par == NULL || lex == NULL) {

		fprintf(stderr, "add_param_data failed\n" );
		return false;
	}

	par->name = lex;

	return true;
}

// add type of the parameter
// return true if success, false if not
bool add_param_type(Par* par, int type) {

	if (par == NULL) {

		fprintf(stderr, "add_param_type failed\n" );
		return false;
	}

	par->type = type;

	return true;
}

// allocates and inicializes return value and adds it to the linked list
// return pointer to Ret if success, NULL if not
Ret* malloc_ret_val(ht_item *item) {

	if (item == NULL) {
		fprintf(stderr, "malloc_ret_val failed\n");
		return NULL;
	}

	Ret * ret_val = malloc(sizeof(Ret));

	if (ret_val == NULL) {
		fprintf(stderr, "Malloc parameter failed\n");
		return NULL;
	}

	ret_val->next = NULL;
	ret_val->type = 0;

	// first return value
	if (item->return_val == NULL) {
		item->return_val = ret_val;
	}

	// there are already some
	else {

		// find last parameter, add it there
		Ret * found = item->return_val;
		for (Ret * tmp = found; tmp != NULL; tmp = tmp->next) {
			found = tmp;
		}

		found->next = ret_val;
	}

	return ret_val;

}

// add type of the return value
// return true if success, false if not
bool add_ret_type(Ret* ret, int type) {

	if (ret == NULL) {

		fprintf(stderr, "add_ret_type failed\n" );
		return false;
	}

	ret->type = type;

	return true;
}


// help function to clean one row
// return true if success, false if not
bool clean_row(ht_item * first, sym_tab *st) {

	if (first == NULL || st == NULL) {
		fprintf(stderr, "clean_row failed\n" );
		return false;
	}

	for (ht_item * tmp = first; tmp != NULL; ){

		ht_item * to_be_deleted = tmp;
		tmp = tmp->next;
		free(to_be_deleted);
		st->size = st->size -1;
	}

	return true;
}

// remove all items from the table
// return the number of items for control purpose, -1 if something failed
int clean_table(sym_tab *st) {

	if (st == NULL) {
		fprintf(stderr, "clean_table failed\n" );
		return -1;
	}

	for (size_t i = 0; i < st->arr_size; i++) {

		// no more items left
		if (st->size == 0) {
			return 0;
		}

		// if no items
		if (st->ptr[i] == NULL) {
			continue;
		}

		if (clean_row(st->ptr[i], st) != true) {
			fprintf(stderr, "clean_table failed\n" );
			return -1;
		}

	}

	return st->size;

 }

// free table from memory
void free_table(sym_tab *st) {

	if (st != NULL) {

		free(st);
	}
}
