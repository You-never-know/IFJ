#include "sem_analysis.h"

enum lex_units id_type_search(sym_list * list_of_tables,lex_unit_t * name){

	ht_item *act=sl_search(list_of_tables,name);
	
	if(act==NULL){ // item is not defined or accesible 
		return ERROR; 
	}
	else{
		if(act->id==NULL)return ERROR;
		return act->id->type; //return correct type
	}
}



enum lex_units tree_check(d_node * rh_node,sym_list * list_of_tables){

	if(rh_node->left==NULL && rh_node->right==NULL){
		if(rh_node->type==EN)
			return id_type_search(list_of_tables,rh_node->data);
		else return ERROR;
	}
	

	if(rh_node->right!=NULL && rh_node->left!=NULL){
		
		if(rh_node->right->type==OP && rh_node->left->type==OP){
			return ERROR; // tree is build incorectly
		}
		if(rh_node->right->type==EN && rh_node->left->type==OP){ //left side OP right side ID
			
			if(id_type_search(list_of_tables,rh_node->right->data)== 
				tree_check(rh_node->left,list_of_tables)) 
				return id_type_search(list_of_tables,rh_node->right->right->data);
			else
				return ERROR;
		}

		if(rh_node->left->type==EN && rh_node->right->type==OP){ //left side ID right side OP
			
			if(id_type_search(list_of_tables,rh_node->left->data)==
				tree_check(rh_node->right,list_of_tables)) 
				return id_type_search(list_of_tables,rh_node->left->data);
			else
				return ERROR;

		}
		if(rh_node->left->type==EN && rh_node->right->type==EN){ //both sides ID
			if(id_type_search(list_of_tables,rh_node->left->data)==
				id_type_search(list_of_tables,rh_node->right->data))
				return id_type_search(list_of_tables,rh_node->right->data);
			else
				return ERROR;
		}
	}

	if(rh_node->right==NULL && rh_node->left!=NULL){

		if(rh_node->left->type==EN)
			return id_type_search(list_of_tables,rh_node->left->data);
		else 
			return ERROR; // tree is build incorrectly
				
	}
	if(rh_node->left==NULL && rh_node->right!=NULL){

		if(rh_node->right->type==EN) //have to be ID 
			return id_type_search(list_of_tables,rh_node->right->data);
		else 
			return ERROR; // tree is build incorrectly
				
	}

	return ERROR; // this should not happen

}

unsigned Sem_analysis(d_node * node,sym_tab * main,sym_list * list_of_tables,lex_unit_t* func_name,bool should_return){

	if(node==NULL)return SYSTEM_ERROR; //  tree is build incorrectly

	if(node->data==NULL)return SYSTEM_ERROR;

	if(node->data->unit_type!=ASSIGNMENT)return SYSTEM_ERROR; // checks if root is =

	if(node->right==NULL)return SYSTEM_ERROR; // no id or value to assign

	if(list_of_tables==NULL)return SYSTEM_ERROR; // no comment ;)


	enum lex_units right_sd=tree_check(node->right,list_of_tables); // chceck derivation tree

	for(d_node * tmp=node->left;tmp!=NULL;tmp=next_left(tmp)){ //pushing left side of tree
		if(id_type_search(list_of_tables,tmp->data)!=right_sd) // comp data types 
			return COMPATIBLE_ERR;
	}

	return SEM_PASSED; // should be ok
 	
}