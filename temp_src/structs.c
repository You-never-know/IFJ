
/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Author:
* Fabo Matúš			xfabom01
**/

#include "structs.h"

lex_unit_t* LexUnitCreate(){
	return malloc(sizeof(lex_unit_t));
}

void LexUnitCtor(lex_unit_t* unit){
	/// Check function argument
	if(unit == NULL) return;

	/// Set all 'unit' variables to default values
	unit -> unit_type = ERROR;
	unit -> data_size = 0;
	unit -> data = NULL;
	unit -> table = NULL;
}

void LexUnitClear(lex_unit_t* unit){
	/// Check function argument
	if(unit == NULL || unit->data == NULL) return;

	/// Free lexical unit data
	free(unit->data);

	/// Set 'unit' variables to default
	unit -> unit_type = ERROR;
	unit -> data_size = 0;
	unit -> data = NULL;
}

void LexUnitDelete(lex_unit_t* unit){
	/// Check function argument
	if(unit == NULL) return;

	/// Free allocated space
	if(unit->data != NULL) free(unit->data);
	free(unit);
}