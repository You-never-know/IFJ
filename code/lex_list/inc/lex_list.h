/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Author:
* Kateøina Neprašová			xnepra01
**/

#ifndef LEX_LIST
#define LEX_LIST

#include "structs.h" 
#include "d_tree.h"
#include <stdio.h>

typedef struct ll_elem { /* lex_list element */
	d_node* ll_data;   /* data */
	struct ll_elem* r;   /* pointer to the next element of the list */
} *ll_elem_ptr;

typedef struct {          /* lex_list */
	ll_elem_ptr first;    /* pointer to the first element of the list */   
	int length;	          /* number of elements */
} lex_list;


void ll_err();
lex_list* ll_init();
void ll_dissolve(lex_list*);
void ll_insert_first(lex_list*, lex_unit_t*);
void ll_del_first(lex_list*);
ll_elem_ptr* ll_return_first(lex_list*);
d_node* ll_return_first_data(lex_list*);
int ll_get_length(lex_list*);

#endif