/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
*
**/

#include "symtable.h"
#include "syntactic_analyzer.h"
#include "sym_list.h"
#include "lexical_analyzer.h"
#include "d_tree.h"
#include <stdbool.h>
#include "lex_list.h"
#include "prec_parser.h"

token_list * Active_token = NULL;
sym_tab* fun_table = NULL;


lex_unit_t * getNextToken() {

	if (Active_token == NULL) {
		return NULL;
	}

	Active_token = Active_token->next;
	
	if (Active_token == NULL) {
		return NULL;
	}

	lex_unit_t * to_be_returned = Active_token->unit;

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
		return NL4(getNextToken()); //skipping new lines
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

	if(!strcmp(act->data,")"))return true; // empty par list

	/* id */

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
bool ret_list(lex_unit_t * act){
	
	if(act==NULL)return false;

	/* end of ret list */

	if(!strcmp(act->data,")"))return true;

	/* "," separate types */

	if(strcmp(act->data,","))return false;

	/* required type */

	if(!type(getNextToken()))return false;

	return ret_list(getNextToken());

}
bool ret_list_start(lex_unit_t * act){

	if(act==NULL)return false;

	if(!strcmp(act->data,")"))return true; // empty ret list

	if(!type(getActiveToken()))return false;

	return ret_list(getNextToken());


} 
bool ret_vals(lex_unit_t * act){
	if(act==NULL)return false;

	/*colum requried */

	if(strcmp(act->data,"("))return false;

	if(!ret_list_start(getNextToken()))return false;

	act = getActiveToken();

	if(strcmp(act->data,")"))return false;
	else return true;
}

bool expression(lex_unit_t* act) {

	// expression starts
	if (act == NULL)return false;

	//TODO
}

bool body(lex_unit_t* act) {

	// body starts
	if (act == NULL)return false;


	if (!strcmp(act->data, "\n")) //NEW_LINE
		return body22(getNextToken());
	else if (!strcmp(act->data, "return")) //return
		return body23(getNextToken());
	else if (!strcmp(act->data, "if")) //if
		return body24(getNextToken());
	else if (act->unit_type == IDENTIFICATOR) //id
		return body25(act);
	else if (!strcmp(act->data, "for")) //for
		return body26(getNextToken());
	else if (!strcmp(act->data, "}")) // end of body
		return true;

	return false;
}

bool body22(lex_unit_t* act) {

	// body22 starts
	if (act == NULL)return false;

	//NEW_LINE DONE IN BODY

	//<NL>
	if (!NL4(act))return false;

	return body(getNextToken());
}

bool body23(lex_unit_t* act) {

	// body23 starts
	if (act == NULL)return false;

	//RETURN DONE IN BODY

	//<exp_list_start>
	if (!exp_list_start(act))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body(getNextToken());
}

bool body24(lex_unit_t* act) {

	// body24 starts
	if (act == NULL)return false;

	//IF DONE IN BODY

	//<expression>
	if (!expression(act))return false; //<expression> todo

	//{
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "{"))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	//<body>
	act = getNextToken();
	if (act == NULL)return false;
	if (!body(act))return false;

	//}
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "}"))return false;

	//<else>
	act = getNextToken();
	if (act == NULL)return false;
	if (!strcmp(else_r(act)))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body(getNextToken());
}

bool body25(lex_unit_t* act) {

	// body25 starts
	if (act == NULL)return false;

	//ID DONE IN BODY

	//<id_choose> 
	if (!id_choose(act))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body(getNextToken());
}


bool body26(lex_unit_t* act) {

	// body26 starts
	if (act == NULL)return false;

	//FOR DONE IN BODY

	//<definition>
	if (!definition(act))return false;

	//;
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, ";"))return false;

	//<expression>
	act = getNextToken();
	if (act == NULL)return false;
	if (!expression(act))return false;

	//;
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, ";"))return false;

	act = getNextToken();
	if (act == NULL)return false;
	if (!assignment(act))return false;

	//{
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "{"))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	//<body>
	act = getNextToken();
	if (act == NULL)return false;
	if (!body(act))return false;

	//}
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "}"))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body(getNextToken());
}


bool id_list(lex_unit_t* act) {

	// id_list starts
	if (act == NULL)return false;

	//eps
	if (!strcmp(act->data, "="))return true;

	//,
	if (strcmp(act->data, ","))return false;

	//ID
	act = getNextToken();
	if (act == NULL)return false;
	if (act->unit_type != IDENTIFICATOR)return false;

	return id_list(getNextToken());
}

bool id_choose(lex_unit_t* act) {

	// id_choose starts
	if (act == NULL)return false;

	lex_unit_t* act_tmp = act; //ID
	act = getNextToken();
	if (act == NULL)return false;

	if (!strcmp(act->data, ":=")) //:=
		return id_choose29(getNextToken());
	else if (id_list(act)) //<id_list>
		return id_choose30(getNextToken());
	else if (!strcmp(act->data, "("))
		return id_choose31(act_tmp); 
	//TODO
	else if
		return true;

	return false;
}

bool id_choose29(lex_unit_t* act) {

	// id_choose29 starts
	if (act == NULL)return false;

	//:= DONE IN ID_CHOOSE

	//<expression>
	if (!expression(act))return false; //<expression> todo

	return true;
}

bool id_choose30(lex_unit_t* act) {

	// id_choose30 starts
	if (act == NULL)return false;

	//<id_list> DONE IN ID_CHOOSE

	//=
	if (strcmp(act->data, "="))return false;

	//<exp/fun>
	act = getNextToken();
	if (act == NULL)return false;
	if (!exp_fun(act))return false;

	return true;
}

bool id_choose31(lex_unit_t* act) {

	// id_choose31 starts
	if (act == NULL)return false;

	// act je ten vstupní token = ID
	
	//TODO
	//volám prec pars 
	//global tlist má (
	//ukazovatel na ukazovatel !
	//make node

	return true;
}

bool else_r(lex_unit_t* act) {

	// id_choose31 starts
	if (act == NULL)return false;

	//eps
	if (!strcmp(act->data, "\n"))return true;

	//else
	if (strcmp(act->data, "else"))return false;

	//{
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "{"))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	//<body>
	act = getNextToken();
	if (act == NULL)return false;
	if (!body(act))return false;

	//}
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "}"))return false;

	return true;
}

bool definition(lex_unit_t* act) {

	//definition starts
	if (act == NULL)return false;

	//ID
	if (act->unit_type != IDENTIFICATOR)return false;

	//:=
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, ":="))return false;

	//<expression>
	act = getNextToken();
	if (act == NULL)return false;
	if (!expression(act))return false; //<expression> todo

	return true;
}

bool assignment(lex_unit_t* act) {

	//assignment starts
	if (act == NULL)return false;

	//ID
	if (act->unit_type != IDENTIFICATOR)return false;

	//<id_list>
	act = getNextToken();
	if (act == NULL)return false;
	if (!id_list(act))return false; //<expression> todo

	//=
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "="))return false;

	//<exp_list_start>
	act = getNextToken();
	if (act == NULL)return false;
	if (!exp_list_start(act))return false;

	return true;
}

bool exp_fun(lex_unit_t* act) {

	//exp_fun starts
	if (act == NULL)return false;

	//<expression>
	if (!expression(act))return false; //<expression> todo

	//<next>
	if (!next(act))return false; 

	return true;
}

bool next(lex_unit_t* act) {

	//next starts
	if (act == NULL)return false;

	//<exp_list>
	if (!exp_list(act))return false;

	return true;
}

bool par_list_start(lex_unit_t* act) { //TODO

	//par_list_start starts
	if (act == NULL)return false;

	return true;
}

bool ret_list_start(lex_unit_t* act) { //TODO

	//ret_list_start starts
	if (act == NULL)return false;

	return true;
}

bool exp_list_start(lex_unit_t* act) { //TODO

	//exp_list_start starts
	if (act == NULL)return false;

	return true;
}

bool fun2(lex_unit_t * act){

	if(act==NULL)return true; //end of file
	
	/* func required */

	if(strcmp(act->data,"func"))return false;

	/* id required */

	act=getNextToken();
	if(act==NULL)return false;
	if(act->unit_type!=IDENTIFICATOR)return false;


	if(!params(getNextToken()))return false;

	if(!ret_vals(getNextToken()))return false;

	/* body of func */

	act=getNextToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"{"))return false;

	/* new line required */

	act=getNextToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"\n"))return false;

	if(!body())return false;



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

	fun_table = function_table;
	Active_token = t_list;

	return true;

} 