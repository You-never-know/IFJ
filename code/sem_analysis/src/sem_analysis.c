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

			case ERROR:
				return COMPATIBLE_ERR;
					
			default:
				return SEM_PASSED;
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

unsigned assignment(d_node * node,sym_tab * main,sym_list * list_of_tables){

	unsigned err; /* semantic err to be returned */
	enum lex_units right_sd; /* data type of tree check */

	if(!strcmp(node->data->data,":=") && !strcmp(node->left->data->data,"_"))
		return OTHER_SEMANTIC;


	for(d_node * tmp=node->left;tmp!=NULL;tmp=next_left(tmp)){ //pushing left side of tree	

			if(node->right!=NULL){ // expression may not occur

				if(!relational_op(node->right->data->data)) // can not be relational operator
					return COMPATIBLE_ERR;
				
				if(tmp->right->data->unit_type==IDENTIFICATOR && func_search(main,node->right->data)!=NULL){
						
						Func * act = func_search(main,node->right->data);

						Ret * return_vals = act->return_val;

						while(return_vals!=NULL && tmp!=NULL){

							if((enum lex_units)return_vals->type!=id_type_search(list_of_tables,tmp->data))
								return (err_sieve(id_type_search(list_of_tables,tmp->data))==SEM_PASSED)? 
										COMPATIBLE_ERR
										: err_sieve(id_type_search(list_of_tables,tmp->data));

							if(tmp->left==NULL && return_vals->next!=NULL) 
									return RETURN_ERR;

							if(tmp->left!=NULL && strcmp(tmp->left->data->data,"_") && return_vals->next==NULL)
									return RETURN_ERR;

							return_vals = return_vals->next;

							tmp=next_left(tmp);


						}
				}	
				else{ 

					right_sd=tree_check(node->right,list_of_tables); // chceck derivation tree 

					err=err_sieve(right_sd); /* possible err */

					if(err!=SEM_PASSED)
					return err;

					if(id_type_search(list_of_tables,tmp->data)!=right_sd) 
						return (err_sieve(id_type_search(list_of_tables,tmp->data))==SEM_PASSED)? 
								COMPATIBLE_ERR
								: err_sieve(id_type_search(list_of_tables,tmp->data));
				
			}
		}

	}

		return SEM_PASSED; // should be ok		
				
}

unsigned if_case(d_node * node,sym_list * list_of_tables){

	unsigned err; /* semantic err to be returned */
	enum lex_units right_sd; /* data type of tree check */

	if(relational_op(node->right->data->data)) // must be relational operator
			return COMPATIBLE_ERR;

	right_sd=tree_check(node->right,list_of_tables); // chceck derivation tree 

	err=err_sieve(right_sd); /* possible err */

	if(err!=SEM_PASSED)
		return err;

	if(id_type_search(list_of_tables,node->data)!=right_sd) 
		return (err_sieve(id_type_search(list_of_tables,node->data))==SEM_PASSED)? 
				COMPATIBLE_ERR
				: err_sieve(id_type_search(list_of_tables,node->data));

	return SEM_PASSED;

}

unsigned return_case(d_node * node,sym_tab * main,sym_list * list_of_tables,lex_unit_t* func_name){

		unsigned err; /* semantic err to be returned */
		enum lex_units right_sd; /* data type of tree check */

		Func * act_func=func_search(main,func_name);
		
		if(act_func==NULL && (act_func->return_val==NULL && node->left!=NULL)) /* return required */
			return RETURN_ERR;

		Ret * return_types=act_func->return_val;

		for(d_node * tmp=node->left;tmp!=NULL && return_types!=NULL;tmp=next_left(tmp)){

			right_sd=tree_check(node->right,list_of_tables);

			err=err_sieve(right_sd); /* possible err */

			if(err!=SEM_PASSED)
				return err;

			if((enum lex_units)return_types->type!=right_sd)
					return err;

			if(tmp->left==NULL && return_types->next!=NULL) /* both have to end with NULL */
				return RETURN_ERR;
			if(tmp->left!=NULL && return_types->next==NULL)
				return RETURN_ERR;

			return_types=return_types->next;

		}

		return SEM_PASSED;

}

unsigned Sem_analysis(d_node * node,sym_tab * main,sym_list * list_of_tables,lex_unit_t* func_name){

	if(node==NULL)return SYSTEM_ERROR; //  tree is build incorrectly

	if(node->data==NULL)return SYSTEM_ERROR;

	if(node->left==NULL && strcmp(node->data->data,"if"))return SYSTEM_ERROR;

	if(list_of_tables==NULL)return SYSTEM_ERROR; // no comment ;)


	if(!strcmp(node->data->data,"=") || !strcmp(node->data->data,":="))
		return assignment(node,main,list_of_tables);

	if(!strcmp(node->data->data,"if"))
		return if_case(node,list_of_tables);

	if(!strcmp(node->data->data,"return"))
		return return_case(node,main,list_of_tables,func_name);

		
  	return SYSTEM_ERROR; 
 	
}