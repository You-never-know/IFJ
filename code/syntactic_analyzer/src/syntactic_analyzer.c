/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
*
**/

#include "structs.h"
#include "symtable.h"
#include "sym_list.h"
#include "lexical_analyzer.h"
#include "d_tree.h"
#include <stdbool.h>
#include "lex_list.h"
#include "prec_parser.h"

token_list * Active_token = NULL;


lex_unit_t * getNextToken() {

	if (Active_token == NULL) {
		return NULL;
	}

	lex_unit_t * to_be_returned = Active_token->unit;
	Active_token = Active_token->next;
	return to_be_returned;
}

lex_unit_t * getActiveToken() {

	if (Active_token == NULL) {
		return NULL;
	}

	return Active_token->unit;
}

/*****************************************************************  Syntactic functions part **********************************************************************/

bool NL5(lex_unit_t *act){

	if(act==NULL)return true; // end of file

	if(!strcmp(act->data,"package") || //only this should be accepted 
	   !strcmp(act->data,"func")	||
	   !strcmp(act->data,"if")){
		return true;
	}
	else
		return false;
}

bool NL4(lex_unit_t *act){

	if(act==NULL)return false;

	if(!strcmp(act->data,"\n")){
		return NL4(getNextToken()); //skipping NEW LINES
	}
	else{
		return NL5(act); //got breakpoint
	}
}

bool type(lex_unit_t * act){

	if(act==NULL)return false;

	if(!strcmp(act->data,"int")||   
	   !strcmp(act->data,"float64")||
	   !strcmp(act->data,"string"))
		return true;
	else
		return false;
}

bool par_list(lex_unit_t * act){

	if(act==NULL)return false;

	if(!strcmp(act->data,")"))return true; // end of param list
	
	/* next param starts */

	if(strcmp(act->data,","))return false;

	/* id */

	act=getNextToken();
	if(act==NULL)return false;
	if(act->unit_type!=IDENTIFICATOR)return false;

	if(!type(getNextToken()))return false;

	return par_list(getNextToken());


}

bool par_list_start(lex_unit_t * act){

	if(act==NULL)return false;

	/* no params */

	if(!strcmp(act->data,")"))return true;

	/* id */

	act=getNextToken();
	if(act==NULL)return false;
	if(act->unit_type!=IDENTIFICATOR)return false;

	if(!type(getNextToken()))return false;

	return par_list(getNextToken()); // returns true only if finds ")"


}

bool params(lex_unit_t * act){

	if(act==NULL)return false;

	/* colum requrired */

	if(strcmp(act->data,"("))return false;

	if(!par_list_start(getNextToken()))return false;
	
	act=getActiveToken();
	
	if(strcmp(act->data,")"))return false; 
	else return true;	
	
}

bool fun2(lex_unit_t * act){

	if(act==NULL)return true; //end of file
	
	/* func required */

	if(strcmp(act->data,"func"))return false;

	/* id required */

	act=getNextToken();
	if(act==NULL)return false;
	if(act->unit_type!=IDENTIFICATOR)return false;

	// TO DO:
	return params(getNextToken());


}
bool prog(){

	/* prog starts */

	lex_unit_t * act = getActiveToken();
	if(act == NULL)return false;

	/* optional new lines */

	if(!NL4(act))return false; 

	/* first have to package */

	act=getActiveToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"package"))return false; 

	/* only main should occur */

	act=getNextToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"main"))return false;

	/* new line required */

	act=getNextToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"\n"))return false;

	/* optional new lines */

	act = getNextToken();
	if(act == NULL)return false;
	if(!NL4(act))return false;

	return fun2(getActiveToken());



}










/*****************************************************************  End of syntactic function part **********************************************************************/

// Main function

bool Check_syntax(token_list * t_list, int * return_value, sym_list * id_tables, sym_tab * function_table) {


	if (t_list == NULL || return_value == NULL || id_tables == NULL || function_table == NULL) {
		if (return_value != NULL) {
			*return_value = 99;
		}
		return false;
	}

	return true;

} 