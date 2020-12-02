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

	/*			   */
	/* TODO: BODY  */
	/*			   */ 

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


bool body() {

	// body starts
	lex_unit_t* act = getActiveToken();
	if (act == NULL)return false;

	
	if (!strcmp(act->data, "\n")) //NEW_LINE
		return body22();
	else if (!strcmp(act->data, "return")) //return
		return body23();
	else if (!strcmp(act->data, "if")) //if
		return body24();
	else if (act->unit_type == IDENTIFICATOR) //id
		return body25();
	else if (!strcmp(act->data, "for")) //for
		return body26();
}

bool body22() {

	// body22 starts
	lex_unit_t* act = getActiveToken();
	if (act == NULL)return false;

	//NEW_LINE DONE IN BODY

	//<NL>
	if (!NL4(act))return false;

	return body();
}

bool body23() {

	// body23 starts
	lex_unit_t* act = getActiveToken();
	if (act == NULL)return false;

	//RETURN DONE IN BODY

	//<exp_list_start>
	act = getNextToken();
	if (act == NULL)return false;
	if (!exp_list_start(act))return false; //<exp_list_start> todo

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body();
}

bool body24() { 

	// body24 starts
	lex_unit_t* act = getActiveToken();
	if (act == NULL)return false;

	//IF DONE IN BODY

	//<expression>
	if (!expression(act))return false; //<expression> todo

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

	//<else>
	act = getNextToken();
	if (act == NULL)return false;
	if (!else(act))return false; // !! RENAME ELSE ? 

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body();
}

bool body25() {

	// body25 starts
	lex_unit_t* act = getActiveToken();
	if (act == NULL)return false;

	//ID DONE IN BODY

	//<id_choose> 
	if (!id_choose(act))return false; //<id_choose> todo

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body();
}


bool body26() {

	// body26 starts
	lex_unit_t* act = getActiveToken();
	if (act == NULL)return false;

	//FOR DONE IN BODY

	//<definition>
	if (!definition(act))return false; //<definition> todo

	//;
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, ";"))return false;

	//<expression>
	act = getNextToken();
	if (act == NULL)return false;
	if (!expression(act))return false; //<expression> todo

	//;
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, ";"))return false;

	//<assignment>
	act = getNextToken();
	if (act == NULL)return false;
	if (!assignment(act))return false; //<assignment> todo

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

	return body();
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