/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Author:
* Kateøina Neprašová			xnepra01
**/

#ifndef SYM_LIST
#define SYM_LIST

#include "symtable.h" 
#include <stdio.h>
#include <stdbool.h> 

typedef struct sl_elem { /* sym_list element */
	sym_tab* st_data;    /* table*/
	struct sl_elem* l;   /* pointer to the previous element of the list */
	struct sl_elem* r;   /* pointer to the next element of the list */
	bool accessible;     /* variable defining accessibility */
} *sl_elem_ptr;

typedef struct {          /* sym_list */
	sl_elem_ptr first;    /* pointer to the first element of the list */
	sl_elem_ptr last;     /* pointer to the last element of the list */
	sl_elem_ptr act;      /* pointer to the actual element of the list */
	int length;	          /* number of elements */
} sym_list;


void sl_err();
sym_list* sl_init();
void sl_dissolve(sym_list*);
void sl_insert_first(sym_list*, sym_tab*);
void sl_insert_last(sym_list*, sym_tab*);
void sl_set_act_first(sym_list*);
void sl_set_act_last(sym_list*);
void sl_set_next_act(sym_list*);
void sl_set_prev_act(sym_list*);
void set_all_accessible(sym_list*);
void sl_set_act_naccessible(sym_list*);
void sl_set_act_accessible(sym_list*);
bool sl_get_act_accessibility(sym_list*);
struct hash_table* sl_return_act(sym_list*);
void sl_del_first(sym_list*);
void sl_del_last(sym_list*);
int sl_get_length(sym_list*);
void sl_rewrite_act(sym_list*, sym_tab*);
struct htable_item* sl_search(sym_list* sl, struct lex_unit* lu);

#endif