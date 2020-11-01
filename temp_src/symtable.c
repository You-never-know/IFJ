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

// find previous item
// return item previous or NULL if no item on the index
ht_item *find_place(lex_unit_t *lex, sym_tab *st) {

	if (st == NULL || lex == NULL) {
		fprintf(stderr, "find_place failed\n");
		return NULL;
	}


	size_t idx = htab_hash_fun((const char*)lex->data) % st->arr_size;

	ht_item * found = st->ptr[idx];
	
	for (ht_item *tmp = found; tmp != NULL; tmp = tmp->next) {
		found = tmp;
	} 

	return found;
}

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
	new->name = lex;

	new->is_function = is_function;
	new->data = NULL;

	new->parameters = NULL;
	new->return_val = NULL;
	new->next = NULL;

	if (new == NULL) {
		fprintf(stderr, "Symtable item malloc error\n");
		return NULL;
	}

	ht_item* place = find_place(lex, st);

	if (place == NULL) {
		add_item_to_the_start(st, lex, new);
		return new;
	}

	place->next = new;

	return new;

}




 