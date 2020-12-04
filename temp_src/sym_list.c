/**
* Project:
* Implementace překladače imperativního jazyka IFJ20
*
* Author:
* Kateřina Neprašová			xnepra01
**/

#include "sym_list.h"
#include "structs.h"
#include "symtable.h"

/*
* Prints err
*/
void sl_err() {

	printf("sym_list ERR\n");
	return;
}

/*
* sym_list initialization
*/
sym_list* sl_init() {

	//set all pointers to NULL
	sym_list* sl = malloc(sizeof(sym_list));
	if (sl == NULL) {
		sl_err();
		return NULL;
	}

	sl->first = NULL;
	sl->last = NULL;
	sl->act = NULL;

	sl->length = 0;

	return sl;
}

/*
* sym_list dissolution
*/
void sl_dissolve(sym_list* sl) {

	sl_elem_ptr tmp = sl->first;

	//dissolute all items
	while (tmp != NULL) {

		sl->first = tmp->r;

		//tmp contents set
		tmp->l = NULL;
		tmp->r = NULL;
		clean_table(tmp->st_data);
		free_table(tmp->st_data);

		free(tmp);

		tmp = sl->first;
	}

	//set items to NULL
	sl->act = NULL;
	sl->first = NULL;
	sl->last = NULL;

	sl->length = 0;

	free(sl);
}

/*
* Inserts the sym_tab* st_data as the first element of sym_list* sl
*/
void sl_insert_first(sym_list* sl, sym_tab* st_data) {

	if ((sl == NULL) || (st_data == NULL)) {
		return;
	}

	//new item allocation
	sl_elem_ptr tmp = (sl_elem_ptr)malloc(sizeof(struct sl_elem));

	//malloc check
	if (tmp == NULL) {
		sl_err();
		return;
	}

	//tmp contents set
	tmp->st_data = st_data;
	tmp->r = sl->first;
	tmp->l = NULL;
	tmp->accessible = true;

	if (sl->first != NULL) { //item exists
		sl->first->l = tmp; //point at new item to the left

	}
	else { //insert to empty list
		sl->last = tmp;
	}
	sl->first = tmp; //set new item tmpPtr as the first one

	(sl->length)++;
}

/*
* Inserts the sym_tab* st_data as the last element of sym_list* sl
*/
void sl_insert_last(sym_list* sl, sym_tab* st_data) {

	if ((sl == NULL) || (st_data == NULL)) {
		return;
	}

	//new item allocation
	sl_elem_ptr tmp = (sl_elem_ptr)malloc(sizeof(struct sl_elem));

	//malloc check
	if (tmp == NULL) {
		sl_err();
		return;
	}

	//tmp contents set
	tmp->st_data = st_data;
	tmp->r = NULL;
	tmp->l = sl->last;
	tmp->accessible = true;

	if (sl->last != NULL) { //item exists
		sl->last->r = tmp; //point at new item to the right
	}
	else {//insert to empty list
		sl->first = tmp;
	}
	sl->last = tmp; //set new item tmpPtr as the last one

	(sl->length)++;
}


/*
* Set the first item as active
*/
void sl_set_act_first(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	sl->act = sl->first;

}

/*
* Set the last item as active
*/
void sl_set_act_last(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	sl->act = sl->last;

}

/*
* Moves the active element to the right
*/
void sl_set_next_act(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	if (sl->act)
	{
		if (sl->act->r != NULL) {
			sl->act = sl->act->r;
		}

	}
}

/*
* Moves the active element to the left
*/
void sl_set_prev_act(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	if (sl->act)
	{
		if (sl->act->l != NULL) {
			sl->act = sl->act->l; 
		}

	}
}

/*
* Sets all sym_list* sl items as accessible
*/
void set_all_accessible(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	sl_elem_ptr tmp = sl->first;

	while (tmp != NULL) {
		tmp->accessible = true;
		tmp = tmp->r;
	}
}

/*
* Sets active item as non-accessible
*/
void sl_set_act_naccessible(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	sl->act->accessible = false;

}

/*
* Sets active item as accessible
*/
void sl_set_act_accessible(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	sl->act->accessible = true;

}

/*
* Returns accessibility of the actual item of sym_list* sl
*/
bool sl_get_act_accessibility(sym_list* sl) {

	if (sl == NULL) {
		return NULL;
	}

	return sl->act->accessible;

}

/*
* Returns data of the actual item of sym_list* sl
*/
sym_tab* sl_return_act(sym_list* sl) {

	if (sl == NULL) {
		return NULL;
	}

	//list is empty
	if (!sl->act)
	{
		sl_err();
		return NULL;
	}

	return sl->act->st_data; //return actual element

}

/*
* Deletes the first element of sym_list* ll
*/
void sl_del_first(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	sl_elem_ptr tmp;

	if (sl->first != NULL) { //list is not empty

		tmp = sl->first;
		if (sl->act == sl->first) { //the first item was active
			sl->act = NULL; //uset active item
		}
		if (sl->first == sl->last) {//dissolve only item
			sl->first = NULL;
			sl->last = NULL;
		}
		else {
			sl->first = sl->first->r; //set the first item to the next one
			sl->first->l = NULL; //pointer to the left set to NULL
		}

		//set pointers to NULL
		tmp->l = NULL;
		tmp->r = NULL;
		tmp->st_data = NULL;

		free(tmp); //release

		(sl->length)--;
	}

}

/*
* Deletes the last element of sym_list* ll
*/
void sl_del_last(sym_list* sl) {

	if (sl == NULL) {
		return;
	}

	sl_elem_ptr tmp;
	if (sl->last != NULL) {//list is not empty
		tmp = sl->last;
		if (sl->act == sl->last) {//the last item was active
			sl->act = NULL; //uset active item
		}
		if (sl->first == sl->last) {//dissolve only item
			sl->first = NULL;
			sl->last = NULL;
		}
		else {
			sl->last = sl->last->l; //set the last item to the previous one
			sl->last->r = NULL; //pointer to the right set to NULL
		}

		//set pointers to NULL 
		tmp->l = NULL;
		tmp->r = NULL;
		tmp->st_data = NULL;

		free(tmp); //release

		(sl->length)--;
	}

}

/*
* Returns length of sym_list* sl
*/
int sl_get_length(sym_list* sl) {

	if (sl == NULL) {
		return -1;
	}

	return sl->length;
}

/*
* Overwrites the contents of the active element of sym_list* sl with sym_tab* st_data
*/
void sl_rewrite_act(sym_list* sl, sym_tab* st_data) {

	if ((sl == NULL) || (st_data == NULL)) {
		return;
	}

	if (sl->act)
	{
		sl->act->st_data = st_data; 
	}
}

/*
* Searches for lex_unit* lu in sym_list* sl
* Returns NULL if param NULL or if not found otherwise the last found ht_item*
*/
ht_item* sl_search(sym_list* sl, struct lex_unit* lu) {

	if ((sl == NULL) || (lu == NULL)) {
		return NULL;
	}

	ht_item* tmp = NULL;

	sl_elem_ptr now = sl->act;

	if (sl->first != NULL) { //list is not empty

		sl_set_act_first(sl);

		for (int i = 0; i < sl->length; i++) {

			if (sl_get_act_accessibility(sl)) {

				if (find_item(sl->act->st_data, lu) != NULL) {
					tmp = find_item(sl->act->st_data, lu);
				}

			}
			sl_set_next_act(sl);
		}
		sl->act = now;
	}

	return tmp;
}
