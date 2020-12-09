/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Author:
* Kateřina Neprašová			xnepra01
**/

#include "lex_list.h"
#include "structs.h"

/*
* Prints err
*/
void ll_err() {

	printf("lex_list ERR\n");
	return;
}

/*
* lex_list initialization
*/
lex_list* ll_init() {

	
	lex_list* ll = malloc(sizeof(lex_list)); //memory allocation
	if (ll == NULL) { //malloc check
		ll_err();
		return NULL;
	}

	//set to NULL
	ll->first = NULL;
	ll->length = 0;

	return ll;
}


/*
* lex_list dissolution
*/
void ll_dissolve(lex_list* ll) {

	ll_elem_ptr tmp = ll->first;
	//dissolute all items
	while (tmp != NULL) {

		ll->first = tmp->r;
		if(tmp->r != NULL){
			//tmp contents set
			delete_tree(tmp->ll_data);
			tmp->r = NULL;

			free(tmp);
		}
		tmp = ll->first;
	}
	//set to NULL
	ll->first = NULL;
	ll->length = 0;

	free(ll);
}

/*
* Inserts the d_node* ll_data as first element of lex_list* ll
*/
void ll_insert_first(lex_list* ll, d_node* ll_data) {

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
	ll->first = tmp; //set new item tmp as the first one

	(ll->length)++;
}

/*
* Deletes the first element of lex_list* ll
*/
void ll_del_first(lex_list* ll) {

	ll_elem_ptr tmp;

	if (ll->first != NULL) { //list is not empty

		tmp = ll->first;

		if (ll->first->r != NULL) {
			ll->first = ll->first->r; //set the first item to the next one
		}

		//set pointers to NULL
		tmp->r = NULL;
		tmp->ll_data = NULL;

		free(tmp); //release

		(ll->length)--;

		if(ll->length ==0){
			ll->first=NULL;
		}
	}

}

/*
* Returns the first element
*/
ll_elem_ptr * ll_return_first(lex_list* ll) {

	return &ll->first;
}

/*
* Returns the data of the first element
*/
d_node* ll_return_first_data(lex_list* ll) {

	return ll->first->ll_data;
}

/*
* Returns length of sym_list
*/
int ll_get_length(lex_list* ll) {

	return ll->length;
}
