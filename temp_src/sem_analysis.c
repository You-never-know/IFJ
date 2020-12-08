#include "sem_analysis.h"

enum lex_units id_type_search(sym_list * list_of_tables,lex_unit_t * name){

	ht_item *act=sl_search(list_of_tables,name);

	if(act==NULL || act->id==NULL || !act->id->accesible){ // item is not defined or accesible 
		return STR_ERR;

	}
	else{
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

unsigned main_fun(sym_tab * main){

	lex_unit_t main_func;
	main_func.unit_type = IDENTIFICATOR;
	main_func.data = "main";
	main_func.data_size = 4;
	main_func.table = NULL;

	ht_item *act=find_item(main,&main_func);

	if(act==NULL || act->func==NULL )
		return OTHER_SEMANTIC;
	else{
		
		if(act->func->parameters !=NULL ||
		   act->func->return_val !=NULL )
			return RETURN_ERR;

		return SEM_PASSED;
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

bool is_err(enum lex_units err){

	switch (err){

		case STR_ERR:
			return true;
		case ERROR:
			return true;
		default:
			return false;
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

	if(rh_node==NULL)
		return ERROR;

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

			enum lex_units r_n = tree_check(rh_node->right,list_of_tables);
			enum lex_units l_n = tree_check(rh_node->left,list_of_tables);

		    if(r_n!=l_n){

		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

		    return r_n;		   
		 
		}

		if(rh_node->right->data->unit_type==IDENTIFICATOR && op(rh_node->left->data->data)){ //left side OP right side ID
			

			enum lex_units r_n = id_type_search(list_of_tables,rh_node->right->data);
			enum lex_units l_n = tree_check(rh_node->left,list_of_tables);

			if(r_n!=l_n){
		    	
		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

			 return r_n;
			
			/* err data type compatibility */


		}

		if(op(rh_node->right->data->data) && rh_node->left->data->unit_type==IDENTIFICATOR){ //left side ID right side OP
			

			enum lex_units r_n = tree_check(rh_node->right,list_of_tables);
			enum lex_units l_n = id_type_search(list_of_tables,rh_node->left->data);

			if(r_n!=l_n){
		    	
		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

			 return r_n;

		}

		if(data_type(rh_node->right->data->unit_type) && op(rh_node->left->data->data)){

			enum lex_units r_n = rh_node->right->data->unit_type;
			enum lex_units l_n = tree_check(rh_node->left,list_of_tables);

			if(r_n!=l_n){
		    	
		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

			 return r_n;

			
		}


		if(op(rh_node->right->data->data) && data_type(rh_node->left->data->unit_type)){

			enum lex_units r_n = tree_check(rh_node->right,list_of_tables);
			enum lex_units l_n = rh_node->left->data->unit_type;

			if(r_n!=l_n){
		    	
		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

			 return r_n;

		}


		if(rh_node->left->data->unit_type==IDENTIFICATOR && rh_node->right->data->unit_type==IDENTIFICATOR){ //both sides ID
			
			enum lex_units r_n = id_type_search(list_of_tables,rh_node->right->data);
			enum lex_units l_n = id_type_search(list_of_tables,rh_node->left->data);

			if(r_n!=l_n){
		    	
		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

			 return r_n;

		}

		if(data_type(rh_node->left->data->unit_type) && data_type(rh_node->right->data->unit_type)){

			if(rh_node->left->data->unit_type==rh_node->right->data->unit_type)
				return rh_node->right->data->unit_type;

			/* err data type compatibility */
			return ERROR;
		}

		if(data_type(rh_node->left->data->unit_type) && rh_node->right->data->unit_type==IDENTIFICATOR){ //both sides ID
			
			enum lex_units r_n = id_type_search(list_of_tables,rh_node->right->data);
			enum lex_units l_n = rh_node->left->data->unit_type;

			if(r_n!=l_n){
		    	
		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

			 return r_n;
		}

		if(rh_node->left->data->unit_type==IDENTIFICATOR && data_type(rh_node->right->data->unit_type)){ //both sides ID
			
			enum lex_units r_n = rh_node->right->data->unit_type;
			enum lex_units l_n = id_type_search(list_of_tables,rh_node->left->data);

			if(r_n!=l_n){
		    	
		    	if(!is_err(r_n) && !is_err(l_n))
		    		return ERROR;
		    	else if(is_err(r_n))
		    		return r_n;
		    	else
		    		return l_n;
		    }

			 return r_n;
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

unsigned assignment_exp(d_node * node,sym_list * list_of_tables){

	if(node->left==NULL)return SYSTEM_ERROR;


	unsigned err; /* semantic err to be returned */
	enum lex_units right_sd; /* data type of tree check */
	if(!strcmp(node->data->data,":=")){

		if(!strcmp(node->left->data->data,"_"))
			return OTHER_SEMANTIC;

		ht_item *act=sl_search(list_of_tables,node->left->data);
		if(act==NULL || act->id==NULL)return SYSTEM_ERROR;

		act->id->accesible=true;
	}



	for(d_node * tmp=node->left;tmp!=NULL;tmp=next_left(tmp)){ //pushing left side of tree	
			
			if(tmp->right==NULL)
				return OTHER_SEMANTIC;

			if(relational_op(tmp->right->data->data)) // can not be relational operator
					return COMPATIBLE_ERR;

			right_sd=tree_check(tmp->right,list_of_tables); // chceck derivation tree 

			err=err_sieve(right_sd); /* possible err */


			if(err!=SEM_PASSED)
				return err;

			if(strcmp(node->left->data->data,"_"))
			if(id_type_search(list_of_tables,tmp->data)!=right_sd) 
					return (err_sieve(id_type_search(list_of_tables,tmp->data))==SEM_PASSED)? 
								COMPATIBLE_ERR
								: err_sieve(id_type_search(list_of_tables,tmp->data));

			printf("X\n");
			
	}

		return SEM_PASSED; // should be ok		
				
}

unsigned assignment_func(d_node * node,sym_tab * main,sym_list * list_of_tables){

	/* special case */


	if(!strcmp(node->data->data,":=") && !strcmp(node->left->data->data,"_")){
		return OTHER_SEMANTIC;
	}


		Func * act = func_search(main,node->right->data);

		Ret * return_vals = act->return_val;

		if(return_vals==NULL)
			return RETURN_ERR; 


						/*return checks*/
	for(d_node * tmp=node->left;tmp!=NULL && return_vals!=NULL;tmp=next_left(tmp)){ //pushing left side of tree	
					
				

				if(strcmp(tmp->data->data,"_"))
				if((enum lex_units)return_vals->type!=id_type_search(list_of_tables,tmp->data))
							return (err_sieve(id_type_search(list_of_tables,tmp->data))==SEM_PASSED)? 
									COMPATIBLE_ERR
									: err_sieve(id_type_search(list_of_tables,tmp->data));

				if(tmp->left==NULL && return_vals->next!=NULL) 
								return RETURN_ERR;

				if(tmp->left!=NULL && strcmp(tmp->left->data->data,"_") && return_vals->next==NULL)
								return RETURN_ERR;
				

				return_vals = return_vals->next;
				
	}

	/* param chcek */

	Par * params = act->parameters;

		if(params == NULL && node->right->left != NULL){
				return PARAM_ERR;
		}

		if(params != NULL && node->right->left == NULL){
				return PARAM_ERR;
		}

	for(d_node * tmp = node->right->left; tmp!=NULL && params!=NULL;tmp=next_left(tmp)){

			if(data_type(tmp->data->unit_type)){
				if((enum lex_units)params->type!=(enum lex_units)tmp->data->unit_type)
					return PARAM_ERR;
			}
			else if(tmp->data->unit_type==IDENTIFICATOR){
				if((enum lex_units)params->type!=id_type_search(list_of_tables,tmp->data))
					return (err_sieve(id_type_search(list_of_tables,tmp->data))==SEM_PASSED)? 
							RETURN_ERR
							: err_sieve(id_type_search(list_of_tables,tmp->data));
			}
			else
				return SYSTEM_ERROR;
	
			if(params->next == NULL && tmp->left != NULL)
					return PARAM_ERR;

			if(params->next != NULL && tmp->left == NULL)
					return PARAM_ERR;

			params=params->next;

	}

 	 return SEM_PASSED;


}

unsigned if_case(d_node * node,sym_list * list_of_tables){


	unsigned err; /* semantic err to be returned */
	enum lex_units right_sd; /* data type of tree check */

	
	if(!relational_op(node->right->data->data)) // must be relational operator
			return COMPATIBLE_ERR;

	right_sd=tree_check(node->right,list_of_tables); // chceck derivation tree 


	err=err_sieve(right_sd); /* possible err */

	if(err!=SEM_PASSED)
		return err;



	return SEM_PASSED;

}

unsigned return_case(d_node * node,sym_tab * main,sym_list * list_of_tables,lex_unit_t* func_name){

		unsigned err; /* semantic err to be returned */
		enum lex_units right_sd; /* data type of tree check */

		Func * act_func=func_search(main,func_name);

		if(act_func==NULL)
			return DEFINE_ERR;
		

		if(act_func->return_val==NULL && node->left!=NULL) /* return required */
			return RETURN_ERR;

		if(act_func->return_val!=NULL && node->left==NULL)
			return RETURN_ERR;

		Ret * return_types=act_func->return_val;

		for(d_node * tmp=node->left;tmp!=NULL && return_types!=NULL;tmp=next_left(tmp)){

			right_sd=tree_check(tmp->right,list_of_tables);

			err=err_sieve(right_sd); /* possible err */


			if(err!=SEM_PASSED)
				return err;

			if((enum lex_units)return_types->type!=right_sd)
						return(err_sieve(right_sd)==SEM_PASSED)? 
							RETURN_ERR
							: err_sieve(right_sd);
				

			if(tmp->left==NULL && return_types->next!=NULL) /* both have to end with NULL */
				return RETURN_ERR;
			if(tmp->left!=NULL && return_types->next==NULL)
				return RETURN_ERR;

		
			return_types=return_types->next;

		}

		return SEM_PASSED;

}

unsigned func_no_return(d_node * node,sym_tab * main,sym_list * list_of_tables){


	Func * act = func_search(main,node->data);
		
	Par * params = act->parameters;

	

	if(params == NULL && node->left != NULL){
				return PARAM_ERR;
	}

	if(params != NULL && node->left == NULL){
				return PARAM_ERR;
	}

	if(act->return_val!=NULL)
		return RETURN_ERR;


	for(d_node * tmp = node->left; tmp!=NULL && params!=NULL;tmp=next_left(tmp)){

			if(data_type(tmp->data->unit_type)){
				if((enum lex_units)params->type!=(enum lex_units)tmp->data->unit_type)
					return PARAM_ERR;
			}
			else if(tmp->data->unit_type==IDENTIFICATOR){
				if((enum lex_units)params->type!=id_type_search(list_of_tables,tmp->data))
					return (err_sieve(id_type_search(list_of_tables,tmp->data))==SEM_PASSED)? 
							COMPATIBLE_ERR
							: err_sieve(id_type_search(list_of_tables,tmp->data));
			}
			else
				return SYSTEM_ERROR;

			if(params->next == NULL && tmp->left != NULL){
				return PARAM_ERR;
			}

			if(params->next!= NULL && tmp->left == NULL){
				return PARAM_ERR;
			}

			params=params->next;


	}

 	 return SEM_PASSED;


}

unsigned Sem_analysis(d_node * node,sym_tab * main,sym_list * list_of_tables,lex_unit_t* func_name){

	if(node==NULL)return SYSTEM_ERROR; //  tree is build incorrectly

	if(node->data==NULL)return SYSTEM_ERROR;

	if(list_of_tables==NULL)return SYSTEM_ERROR; // no comment ;)

	if((!strcmp(node->data->data,"=") || !strcmp(node->data->data,":="))&& node->right==NULL)
		return assignment_exp(node,list_of_tables);

	else if(!strcmp(node->data->data,"=")  && node->right!=NULL)
		return assignment_func(node,main,list_of_tables);

	else if(!strcmp(node->data->data,"if"))
		return if_case(node,list_of_tables);

	else if(!strcmp(node->data->data,"return"))
		return return_case(node,main,list_of_tables,func_name);
	else if(!strcmp(node->data->data,"print"))
		return SEM_PASSED;
	else
		return func_no_return(node,main,list_of_tables);

		
  	return SYSTEM_ERROR; 
 	
}