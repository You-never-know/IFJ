#include "sem_analysis.h"

enum lex_units id_type_search(sym_list * list_of_tables,lex_unit_t * name){

	ht_item *act=sl_search(list_of_tables,name);
	
	if(act==NULL){ // item is not defined or accesible 
		return STR_ERR; 
	}
	else{
		if(act->id==NULL)return ERROR;
		return act->id->type; //return correct type
	}
}


Func * func_search(sym_tab * main,lex_unit_t *name){

	ht_item *act=find_item(main,name);
	
	if(act==NULL){ // item is not defined or accesible 
		return NULL; 
	}
	else{
		if(act->func==NULL)return NULL;
		return act->func;
	}

}

enum lex_units return_type_search(sym_tab * main,lex_unit_t * name){

	Func * act = func_search(main,name);
	if(act==NULL)return ERROR;

	return (act->return_val==NULL)? ERROR : act->return_val->type;

}

bool op(void* data){

	if(data==NULL){
		fprintf(stderr,"data eq NULL");
		return false;
	}
	return 	  (!strcmp(data,"+")  || !strcmp(data,"-") ||
			   !strcmp(data,"/")  || !strcmp(data,"*"));
}

bool data_type(enum lex_units type){

	return 	  (type == INTEGER ||
			   type == STRING  ||
			   type == DECIMAL);
}

bool relational_op(void *data){

	if(data==NULL){
		fprintf(stderr,"data eq NULL");
		return false;
	}

	return 	   (!strcmp(data,"<")  || !strcmp(data,">") ||
			    !strcmp(data,"==") || !strcmp(data,"!=")||
			    !strcmp(data,">=") || !strcmp(data,"<="));
}

unsigned err_sieve(enum lex_units err){

		switch (err){
								/* sieve for correct err */
			case STR_ERR:
				return DEFINE_ERR;
					
			default:
				return COMPATIBLE_ERR;
		}
	}

bool return_not_found(lex_unit_t *name,sym_tab * main){

	Func * act = func_search(main,name);
	if(act==NULL){
		fprintf(stderr,"func not found\n");
		return false;
	}

	return(act->return_val==NULL)?true:false;
	
}

enum lex_units tree_check(d_node * rh_node,sym_list * list_of_tables){

	if(rh_node->left==NULL && rh_node->right==NULL){
		
		if(rh_node->data->unit_type==IDENTIFICATOR)
			return id_type_search(list_of_tables,rh_node->data);
		
		if(data_type(rh_node->data->unit_type))
			return rh_node->data->unit_type;

		/* tree is build incorectly */
		return ERROR;
	}
	

	if(rh_node->right!=NULL && rh_node->left!=NULL){
		
		if(op(rh_node->right->data->data) && op(rh_node->left->data->data)){
			/* tree is build incorectly */
			return ERROR; 
		}

		if(rh_node->right->data->unit_type==IDENTIFICATOR && op(rh_node->left->data->data)){ //left side OP right side ID

			if(id_type_search(list_of_tables,rh_node->right->data)==tree_check(rh_node->left,list_of_tables)) 
					return id_type_search(list_of_tables,rh_node->right->right->data);

			/* err data type compatibility */
			return ERROR;
		}

		if(data_type(rh_node->right->data->unit_type) && op(rh_node->left->data->data)){

			if((enum lex_units)rh_node->right->data->unit_type==tree_check(rh_node->left,list_of_tables))
				return rh_node->right->data->unit_type;

			/* err data type compatibility */
			return ERROR;
		}

		if(op(rh_node->right->data->data) && rh_node->left->data->unit_type==IDENTIFICATOR){ //left side ID right side OP
			
			if(id_type_search(list_of_tables,rh_node->left->data)==
				tree_check(rh_node->right,list_of_tables)) 
				return id_type_search(list_of_tables,rh_node->left->data);

			/* err data type compatibility */
			return ERROR;

		}

		if(op(rh_node->right->data->data) && data_type(rh_node->left->data->unit_type)){

			if((enum lex_units)rh_node->left->data->unit_type==tree_check(rh_node->right,list_of_tables))
				return rh_node->left->data->unit_type;

			/* err data type compatibility */
			return ERROR;
		}


		if(rh_node->left->data->unit_type==IDENTIFICATOR && rh_node->right->data->unit_type==IDENTIFICATOR){ //both sides ID
			
			if(id_type_search(list_of_tables,rh_node->left->data)==
				id_type_search(list_of_tables,rh_node->right->data))
				return id_type_search(list_of_tables,rh_node->right->data);

			/* err data type compatibility */
			return ERROR;
		}

		if(data_type(rh_node->left->data->unit_type) && data_type(rh_node->right->data->unit_type)){

			if(rh_node->left->data->unit_type==rh_node->right->data->unit_type)
				return rh_node->right->data->unit_type;

			/* err data type compatibility */
			return ERROR;
		}
	}

	if(rh_node->right==NULL && rh_node->left!=NULL){

		if(rh_node->left->data->unit_type==IDENTIFICATOR)
			return id_type_search(list_of_tables,rh_node->left->data);

		if(data_type(rh_node->left->data->unit_type))
			return rh_node->left->data->unit_type;

		/* err data type compatibility */
		return ERROR; 
				
	}
	if(rh_node->left==NULL && rh_node->right!=NULL){

		if(rh_node->right->data->unit_type==IDENTIFICATOR)
			return id_type_search(list_of_tables,rh_node->right->data);

		if(data_type(rh_node->right->data->unit_type))
			return rh_node->right->data->unit_type;

		/* err data type compatibility */
		return ERROR; 
				
	}

	/* this should not happen */
	return ERROR; 

}

unsigned Sem_analysis(d_node * node,sym_tab * main,sym_list * list_of_tables,lex_unit_t* func_name){

	if(node==NULL)return SYSTEM_ERROR; //  tree is build incorrectly

	if(node->data==NULL)return SYSTEM_ERROR;

	if(list_of_tables==NULL)return SYSTEM_ERROR; // no comment ;)

	unsigned err; /* semantic err to be returned */
	enum lex_units right_sd; /* data type of tree check */

	if(!strcmp(node->data->data,"=") || !strcmp(node->data->data,"if") || !strcmp(node->data->data,":=")){

		if(!strcmp(node->data->data,"if")){
			if(relational_op(node->right->data->data)) // must be relational operator
				return COMPATIBLE_ERR;
		}

		if(!strcmp(node->data->data,"=") || !strcmp(node->data->data,":=")){
			if(!relational_op(node->right->data->data)) // can not be relational operator
				return COMPATIBLE_ERR;
		}

		for(d_node * tmp=node->left;tmp!=NULL;tmp=next_left(tmp)){ //pushing left side of tree	

			if(node->right!=NULL){ // assignment may not occur
				
				if(tmp->right->data->unit_type==IDENTIFICATOR && func_search(main,node->right->data)!=NULL)
					 right_sd=return_type_search(main,node->right->data); // chcek return type of func
				else right_sd=tree_check(node->right,list_of_tables); // chceck derivation tree 

				err=err_sieve(right_sd); /* possible err */

				if(id_type_search(list_of_tables,tmp->data)!=right_sd) // comp data types 
					return err;
			}

		}

		return SEM_PASSED; // should be ok
	}

	if(!strcmp(node->data->data,"return")){

		Func * act_func=func_search(main,func_name);
		if(act_func==NULL && (act_func->return_val==NULL && node->left!=NULL)) /* return required */
			return RETURN_ERR;

		Ret * return_types=act_func->return_val;

		for(d_node * tmp=node->left;tmp!=NULL && return_types!=NULL;tmp=next_left(tmp)){

			right_sd=tree_check(node->right,list_of_tables);

			err=err_sieve(right_sd); /* possible err */

			if((enum lex_units)return_types->type!=right_sd)
					return err;

			if(tmp->left==NULL && return_types->next!=NULL) /* both have to end with NULL */
				return RETURN_ERR;
			if(tmp->left!=NULL && return_types->next==NULL)
				return RETURN_ERR;

			return_types=return_types->next;


		}


	}


  return SEM_PASSED; 
 	
}