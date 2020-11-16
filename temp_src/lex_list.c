/**
* Project:
* Implementace překladače imperativního jazyka IFJ20
*
* Author:
* Kateřina Neprašová			xnepra01
**/

#include "lex_list.h"
#include "structs.h"

void ll_err() {

	printf("ERROR: The program has performed an illegal operation.\n");
	return;
}

lex_list* ll_init() {

	//set all pointers to NULL
	lex_list* ll = malloc(sizeof(lex_list));
	if (ll == NULL) {
		ll_err();
		return NULL;
	}

	ll->first = NULL;
	ll->last = NULL;
	ll->length = 0;

	return ll;
}

void ll_dissolve(lex_list* ll) {

	ll_elem_ptr tmp = ll->first;

	//dissolute all items
	while (tmp != NULL) {

		ll->first = tmp->r;

		//tmp contents set
		tmp->l = NULL;
		tmp->r = NULL;
		tmp->ll_data = NULL;

		free(tmp);

		tmp = ll->first;
	}

	//set items to NULL
	ll->first = NULL;
	ll->last = NULL;
	ll->length = 0;

	free(ll);
}

void ll_insert_first(lex_list* ll, lex_unit_t* ll_data) {

	//new item allocation
	ll_elem_ptr tmp = (ll_elem_ptr)malloc(sizeof(struct ll_elem));

	//malloc check
	if (tmp == NULL) {
		ll_err();
		return;
	}

	//tmp contents set
	tmp->ll_data = ll_data;
	tmp->r = ll->first;
	tmp->l = NULL;

	if (ll->first != NULL) { //item exists
		ll->first->l = tmp; //point at new item to the left

	}
	else { //insert to empty list
		ll->last = tmp;
	}
	ll->first = tmp; //set new item tmpPtr as the first one

	(ll->length)++;
}


void ll_del_first(lex_list* ll) {

	ll_elem_ptr tmp;

	if (ll->first != NULL) { //list is not empty

		tmp = ll->first;
	
		if (ll->first == ll->last) {//dissolve only item
			ll->first = NULL;
			ll->last = NULL;
		}
		else {
			ll->first = ll->first->r; //set the first item to the next one
			ll->first->l = NULL; //pointer to the left set to NULL
		}

		//set pointers to NULL
		tmp->l = NULL;
		tmp->r = NULL;
		tmp->ll_data = NULL;

		free(tmp); //release

		(ll->length)--;
	}

}


int ll_get_length(lex_list* ll) {

	//return length of sym_list
	return ll->length;
}