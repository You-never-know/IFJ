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
#include "sem_analysis.h"
#include "prec_table.h"
#include <stdbool.h>
#include "lex_list.h"
#include "prec_parser.h"

token_list * Active_token = NULL;
sym_tab* fun_table = NULL;
sym_list * tables = NULL;
int return_code = 0;
int Err_set = 0;
bool was_return = false;
lex_unit_t * func_name = NULL;
d_node * assignment_start = NULL;

void set_return_code(unsigned CODE) {

	if (Err_set == 0) {
		return_code = CODE;
		Err_set++;
	}
}


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


	if (!strcmp(act->data, "package") || //only this should be accepted 
		!strcmp(act->data, "func") ||
		!strcmp(act->data, "if") ||
		!strcmp(act->data, "for") ||
		!strcmp(act->data, "}") ||
		!strcmp(act->data, "return") ||
		act->unit_type == IDENTIFICATOR) {
		return true;
	}
	else
		return false;
}

bool NL4(lex_unit_t *act){

	if(act==NULL)return true;

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

	if(!strcmp(act->data,")")) {
		return true;
	}

	/* "," separate types */

	if(strcmp(act->data,","))return false;

	/* required type */

	if(!type(getNextToken()))return false;

	return ret_list(getNextToken());

}
bool ret_list_start(lex_unit_t * act){

	if(act==NULL)return false;

	if(!strcmp(act->data,")")) {
		return true; // empty ret list
	}
	if(!type(getActiveToken()))return false;

	return ret_list(getNextToken());


} 
bool ret_vals(lex_unit_t * act){
	if(act==NULL)return false;

	if (!strcmp(act->data, "{")) return true;
	/*colum requried */

	if(strcmp(act->data,"("))return false;

	if(!ret_list_start(getNextToken()))return false;

	act = getActiveToken();

	if(strcmp(act->data,")"))return false;
	else {
		getNextToken();
		return true;
	}
}

bool expression(lex_unit_t* act, d_node * root) {

	// expression starts
	if (act == NULL)return false;
	if (Active_token == NULL) return false;
	Active_token = Active_token->next; // get the next token
	bool result = Parse_expresion(act, root, &Active_token, fun_table);
	return result;
}

bool body(lex_unit_t* act) {

	// body starts
	if (act == NULL)return false;

	if (!strcmp(act->data, "\n")) //NEW_LINE
		return body22(getNextToken());
	else if (!strcmp(act->data, "return")) //return
		return body23(getActiveToken());
	else if (!strcmp(act->data, "if")) //if
		return body24(getActiveToken());
	else if (act->unit_type == IDENTIFICATOR) //id
		return body25(act);
	else if (!strcmp(act->data, "for")) //for
		return body26(getActiveToken());
	else if (!strcmp(act->data, "}")) //eps
		return true;

	return false;
}

bool body22(lex_unit_t* act) {

	// body22 starts
	if (act == NULL)return false;

	//NEW_LINE DONE IN BODY

	//<NL>
	if (!NL4(act))return false;

	return body(getActiveToken());
}

bool body23(lex_unit_t* act) {

	// body23 starts
	if (act == NULL)return false;

	d_node * return_tree = d_node_create(NULL, act, E);
	d_node * body1 = d_node_create(NULL, NULL, DOLLAR);
	d_node_insert_left(return_tree, body1);	
	act = getNextToken();

	//RETURN DONE IN BODY
	//<exp_list_start>
	if (!exp_list_start(act, body1)) {                        
		delete_tree(return_tree);
		return false;
	}
	unsigned err = Sem_analysis(return_tree, fun_table, tables, func_name);  /////////////////////////// RETURN TREE
	if (err != 0) {
		set_return_code(err);
	}
	delete_tree(return_tree);

	//NEW_LINE
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;
	return body(getNextToken());
}

bool body24(lex_unit_t* act) {

	// body24 starts
	if (act == NULL)return false;

	//IF DONE IN BODY
	d_node * if_tree = d_node_create(NULL, act, DOLLAR);
	act = getNextToken();
	//<expression>

	bool decide = expression(act, if_tree);

	if (decide == false) {
		delete_tree(if_tree);
		return false;
	}

	unsigned err = Sem_analysis(if_tree, fun_table, tables, func_name); ///////////////////////// IF TREE
	if (err != 0) {
		set_return_code(err);
	}
	delete_tree(if_tree);

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
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "}"))return false;

	//<else>
	act = getNextToken();
	if (act == NULL)return false;
	if (!else_r(act))return false;

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
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;
	
	return body(getNextToken());
}


bool body26(lex_unit_t* act) {

	// body26 starts
	if (act == NULL)return false;

	//FOR DONE IN BODY
	lex_unit_t * for_tmp = act;
	act = getNextToken();

	d_node * for_tree = d_node_create(NULL, for_tmp, DOLLAR);
	d_node * body1 = d_node_create(NULL, NULL, DOLLAR);
	d_node_insert_left(for_tree, body1);
	//<definition>
	if (!definition(act, body1)) {
		delete_tree(for_tree);
		return false;
	}

	//;
	act = getActiveToken();
	if (act == NULL) {
		delete_tree(for_tree);
		return false;
	}
	if (strcmp(act->data, ";")) {
		delete_tree(for_tree);
		return false;
	}

	//<expression>
	act = getNextToken();
	if (act == NULL) {
		delete_tree(for_tree);
		return false;
	}

	d_node * body2 = d_node_create(NULL, NULL, DOLLAR);
	d_node_insert_left(body1, body2);

	bool decide = expression(act, body2);

	if (decide == false) {
		delete_tree(for_tree);
		return false;
	}

	//;
	act = getActiveToken();
	if (act == NULL) {
		delete_tree(for_tree);
		return false;
	}
	if (strcmp(act->data, ";")) {
		delete_tree(for_tree);
		return false;
	}

	d_node * body3 = d_node_create(NULL, NULL, DOLLAR);
	d_node_insert_left(body2, body3);

	//<assignment>
	act = getNextToken();
	if (act == NULL) {
		delete_tree(for_tree);
		return false;
	}
	if (!assignment_38_39(act, body2)) {
		delete_tree(for_tree);
		return false;
	}

	// send to generate code            /////////// /////////////////////////// FOR TREE 
	delete_tree(for_tree);

	//{
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "{")) {
		delete_tree(for_tree);
		return false;
	}

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	//<body>
	act = getNextToken();
	if (act == NULL)return false;
	if (!body(act))return false;

	//}
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "}"))return false;

	//NEW_LINE
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "\n"))return false;

	return body(getNextToken());
}


bool id_list(lex_unit_t* act, d_node * root) {

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
	d_node * new_id = d_node_create(NULL, act, I);
	d_node_insert_left(root, new_id);

	return id_list(getNextToken(), new_id);
}

bool id_choose(lex_unit_t* act) {

	// id_choose starts
	if (act == NULL)return false;
	lex_unit_t* act_tmp = act; //ID
	act = getNextToken();
	if (act == NULL)return false;

	d_node * operator = d_node_create(NULL, NULL, ASSIGNMENT);
	d_node * id = d_node_create(NULL, act_tmp, I);
	d_node_insert_left(operator, id);

	if (!strcmp(act->data, ":=")) {//:=
		operator->data = act;
		bool decide = id_choose29(getNextToken(), operator);

		if (decide == true) {
			unsigned err = Sem_analysis(operator, fun_table, tables, func_name); ///////////////////////// := tree
			if (err != 0) {
				set_return_code(err);
			}

			// send to generate code
		}
		delete_tree(operator);

		return decide;
	}
	else if ((!strcmp(act->data, ",")) || (strcmp(act->data, "=") == 0)) //<id_list>
		return id_choose30(getActiveToken(), operator);
	else if (!strcmp(act->data, "(")) { //(
		delete_tree(operator);
		return id_choose31(act_tmp); 
	}

	return false;
}

bool id_choose29(lex_unit_t* act, d_node * assignment) {

	// id_choose29 starts
	if (act == NULL)return false;

	//:= DONE IN ID_CHOOSE
	//<expression>
	if (!expression(act, assignment))return false; 

	return true;
}

bool id_choose30(lex_unit_t* act, d_node * assignment) {

	// id_choose30 starts
	if (act == NULL || assignment == NULL)return false;

	if (assignment->left == NULL) return false;

	//<id_list> DONE IN ID_CHOOSE
	if (!id_list(act, assignment->left)) return false;

	//=
	act = getActiveToken();
	if (strcmp(act->data, "="))return false;
	assignment->data = act;
	//<exp/fun>
	act = getNextToken();
	if (act == NULL)return false;
	if (!exp_fun(act, assignment))return false; 

	unsigned err = Sem_analysis(assignment, fun_table, tables, func_name); //////////////////////////////////////// = tree
	if (err != 0) {
		set_return_code(err);
	}
	// send to generate code
	delete_tree(assignment);

	return true;
}

bool id_choose31(lex_unit_t* act) {

	// id_choose31 starts
	if (act == NULL)return false;
	// act = ID
	// global t_list : (
	// make node
	d_node* root = NULL;
	root = d_node_create(NULL, NULL, F);

	bool result = Parse_expresion(act, root, &Active_token, fun_table);
	unsigned err = Sem_analysis(root->right, fun_table, tables, func_name); //////////////////////////////////////// Function alone 
	if (err != 0) {
		set_return_code(err);
	}
	// send to generate code
	delete_tree(root);

	return result;
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
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "}"))return false;

	return true;
}

bool definition(lex_unit_t* act, d_node * root) {

	//definition starts
	if (act == NULL)return false;

	//eps
	if (!strcmp(act->data, ";"))return true;
	d_node * equal = d_node_create(NULL, NULL, E);
	d_node_insert_left(root, equal);

	//ID
	if (act->unit_type != IDENTIFICATOR)return false;
	d_node * id = d_node_create(NULL, act, I);
	d_node_insert_left(equal, id);

	//:=
	act = getNextToken();
	if (act == NULL)return false;
	if (strcmp(act->data, ":="))return false;
	equal->data = act;

	//<expression>
	act = getNextToken();
	if (act == NULL)return false;
	if (!expression(act, equal))return false; 

	return true;
}

bool assignment_38_39(lex_unit_t* act, d_node * root) {

	//assignment starts
	if (act == NULL)return false;

	//eps
	if (!strcmp(act->data, "{"))return true;
	d_node * equal = d_node_create(NULL, NULL, E);
	d_node_insert_left(root, equal);
	//ID
	if (act->unit_type != IDENTIFICATOR)return false;
	d_node * id_1 = d_node_create(NULL, act, I);
	
	assignment_start = id_1;
	d_node_insert_left(equal, id_1);

	//<id_list>
	act = getNextToken();
	if (act == NULL)return false;
	if (!id_list(act, id_1))return false;

	//=
	act = getActiveToken();
	if (act == NULL)return false;
	if (strcmp(act->data, "="))return false;
	equal->data = act;
	//<exp_list_start>
	act = getNextToken();
	if (act == NULL)return false;
	if (!exp_list_start(act, assignment_start))return false;

	return true;
}

bool exp_fun(lex_unit_t* act, d_node * id) {

	//exp_fun starts
	if (act == NULL)return false;

	
	if (find_item(fun_table, act) != NULL) {
		//<expression>
		if (!expression(act, id))return false;
	}
	else {
		//<expression>
		id = id->left;
		if (!expression(act, id))return false;
	}
	//<next>
	act = getActiveToken();
	if (act == NULL)return false;
	if (!next(act, id->left))return false; 

	return true;
}

bool next(lex_unit_t* act, d_node * id) {

	//next starts
	if (act == NULL)return false;

	//eps
	if (!strcmp(act->data, "\n"))return true;

	//<exp_list>
	if (!exp_list(act, id))return false;

	return true;
}

bool exp_list(lex_unit_t* act, d_node * id) {

	//exp_list starts
	if (act == NULL)return false;

	//eps
	if (!strcmp(act->data, "\n") || (!strcmp(act->data, "{"))) {
		if (id->type == DOLLAR) {
			delete_tree(id);
		}
		return true;
	}

	//,
	if (strcmp(act->data, ","))return false;
	if (id == NULL) return false;

	//<expression>
	act = getNextToken();
	if (act == NULL)return false;
	if (!expression(act, id))return false;
	act = getActiveToken(); 

	if (id->type == DOLLAR) {
		d_node * body_k = d_node_create(NULL, NULL, DOLLAR);
		d_node_insert_left(id, body_k);
	}

	return exp_list(act, id->left);
}

bool exp_list_start(lex_unit_t* act, d_node * body) { 

	//exp_list_start starts
	if (act == NULL)return false;

	//eps
	if (!strcmp(act->data, "\n"))return true;

	if (((!strcmp(act->data, "(")) || act->unit_type == IDENTIFICATOR || act->unit_type == INTEGER || act->unit_type == STRING || act->unit_type == DECIMAL)) {

		//<expression>
		if (!expression(act, body))return false;

		if (body == DOLLAR) {
			d_node * body2 = d_node_create(NULL, NULL, DOLLAR);
			d_node_insert_left(body, body2);
			body = body2;
		}
		else {
			body = body->left;
		}

		//<exp_list>
		act = getActiveToken();
		if (act == NULL)return false;
		if (!exp_list(act, body))return false;

		return true;
	}
	return false;


}

bool fun2(lex_unit_t * act){
	if(act==NULL)return true; //end of file
	
	/* func required */

	if(strcmp(act->data,"func"))return false; /////////////////////////////////////////////// TO DO function tree for generate code

	/* id required */

	act=getNextToken();
	if(act==NULL)return false;
	if(act->unit_type!=IDENTIFICATOR)return false;
	func_name = act;

	if(!params(getNextToken()))return false;

	if(!ret_vals(getNextToken()))return false;

	/* body of func */

	act=getActiveToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"{"))return false;

	/* new line required */
	act=getNextToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"\n"))return false;
	
	if(!body(getNextToken()))return false;

	// check }
	act=getActiveToken();
	if(act==NULL)return false;
	if(strcmp(act->data,"}"))return false; 

	if (!return_not_found(func_name, fun_table)) set_return_code(7);

	// check NEW_LINE
	act=getNextToken();
	if(act==NULL)return true;
	if(strcmp(act->data,"\n"))return false;

	// call NL
	act=getNextToken();
	if(!NL4(act))return false; 
	
	return fun2(getActiveToken());

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

int Check_syntax(token_list * t_list, sym_list * id_tables, sym_tab * function_table) {


	if (t_list == NULL || id_tables == NULL || function_table == NULL) {
		return 99;
	}

	fun_table = function_table; // set the global 
	Active_token = t_list;
	tables = id_tables;

	if (prog() == false) {
		set_return_code(2); // syntactic error
	}

	return return_code;

} 