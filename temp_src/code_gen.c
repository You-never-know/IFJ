/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Author:
* Fabo Matus	xfabom01
**/


#include "code_gen.h"
#include "stack.h"
#include "structs.h"
#include "prec_table.h"
#include "symtable.h"
#include <string.h>
#include <stdio.h>

char in_function = 0; // Boolean substitute
stack_t* label_stack = NULL;
stack_t* var_stack = NULL; // For frame reference

void retval_move_reverse(d_node* root, FILE* file_descriptor){
	static int r_index = 0;
	if(root == NULL || file_descriptor == NULL){
		r_index = 0;
		return;
	}
	if(root->left != NULL) retval_move_reverse(root->left, file_descriptor);
	char tmp_frame[3] = "LF";
	s_find(var_stack, tmp_frame, (char*)root->data->data);
	fprintf(file_descriptor, "MOVE %s@%s TF@%%retval%d\n", tmp_frame, (char*)root->data->data, r_index++);
}

void expr_unpack(d_node* root, FILE* file_descriptor, sym_list* sl){
	/// Check function arguments
	if(root == NULL || file_descriptor == NULL || var_stack == NULL) return;

	/// Recursively loop through 'root' tree
	char on_stack = 0;
	if(root->right != NULL && root->left != NULL){
		if(root->right->right != NULL && root->right->left != NULL){
			expr_unpack(root->right, file_descriptor, sl);
			on_stack |= 0x0f;
		}
		if(root->left->right != NULL && root->left->left != NULL){
			expr_unpack(root->left, file_descriptor, sl);
			on_stack |= 0xf0;
		}
	}

	/// Decide what to do
	if(root->type == ASSIGNMENT){
		if(((char*)root->data->data)[0] == ':'){
			/// "Compute" the expression
			expr_unpack(root->left->right, file_descriptor, sl);

			/// Set up resources
			int tmp_var_len = 2+root->left->data->data_size;
			char* tmp_lex_data = (char*)root->left->data->data;
			char* tmp_var = malloc(tmp_var_len+3);
			if(tmp_var == NULL){
				fprintf(stderr, "Allocation failed. (Code generation - copying variable)\n");
				return;
			}
			int tmp_index = 0;
			tmp_var[tmp_index++] = 'G';
			if(in_function) tmp_var[tmp_index-1] = 'L';
			tmp_var[tmp_index++] = 'F';

			/// Copy variable name
			while((tmp_var[tmp_index++] = *tmp_lex_data++) != '\0');

			/// "Create" & "initialize" the variable
			s_push(&var_stack, tmp_var, tmp_var_len);
			fprintf(file_descriptor, "DEFVAR %c%c@%s\nPOPS %c%c@%s\n",tmp_var[0], tmp_var[1], (char*)root->left->data->data ,tmp_var[0], tmp_var[1], (char*)root->left->data->data);
			free(tmp_var);
		}
		else{
			if(root->right == NULL){ /// Multiple assignments
				for(d_node* tmp = root->left; tmp != NULL; tmp = tmp->left){
					expr_unpack(tmp->right, file_descriptor, sl);
					char tmp_frame[3] = {0,0,0};
					s_find(var_stack, tmp_frame, (char*)tmp->data->data);
					fprintf(file_descriptor, "POPS %s@%s\n", tmp_frame, (char*)tmp->data->data);
				}
			}
			else{ /// Calling function
				/// "Push" all parameters on stack 
				int par_index = 0;
				for(d_node* tmp = root->right->left; tmp != NULL; tmp = tmp->left){
					/// Find frame scope for parameter
					char* tmp_str_replaced = NULL;
					char tmp_frame[7] = {0,0,0,0,0,0,0};
					s_find(var_stack, tmp_frame, (char*)tmp->data->data);

					if(tmp_frame[0] == '\0'){
						/// Parameter is not a variable
						if(tmp->data->unit_type == STRING){
							tmp_frame[0] = 's';
							tmp_frame[1] = 't';
							tmp_frame[2] = 'r';
							tmp_frame[3] = 'i';
							tmp_frame[4] = 'n';
							tmp_frame[5] = 'g';
							tmp_frame[6] = '\0';
							
							/// Allocate enough space for replacing every character (worst case scenario)
							tmp_str_replaced = malloc((tmp->data->data_size+1)*4);
							if(tmp_str_replaced == NULL){
								fprintf(stderr, "Buffer allocation failed.(Code generation - replacing whitespace characters)\n");
								return;
							}
							int index = 0;

							/// Replace all whitespace characters with their decimal representation
							char* tmp_lex_str = (char*)tmp->data->data;
							while((tmp_str_replaced[index] = *(tmp_lex_str++)) != '\0'){
								if(tmp_str_replaced[index] == ' '){
									tmp_str_replaced[index++] = '\\';
									tmp_str_replaced[index++] = '0';
									tmp_str_replaced[index++] = '3';
									tmp_str_replaced[index]   = '2';
								}
								else if(tmp_str_replaced[index] == '\n'){
									tmp_str_replaced[index++] = '\\';
									tmp_str_replaced[index++] = '0';
									tmp_str_replaced[index++] = '1';
									tmp_str_replaced[index]   = '0';
								}
								else if(tmp_str_replaced[index] == '\t'){
									tmp_str_replaced[index++] = '\\';
									tmp_str_replaced[index++] = '0';
									tmp_str_replaced[index++] = '0';
									tmp_str_replaced[index]   = '9';
								}
								else if(tmp_str_replaced[index] == '\v'){
									tmp_str_replaced[index++] = '\\';
									tmp_str_replaced[index++] = '0';
									tmp_str_replaced[index++] = '1';
									tmp_str_replaced[index]   = '1';
								}
								else if(tmp_str_replaced[index] == '\f'){
									tmp_str_replaced[index++] = '\\';
									tmp_str_replaced[index++] = '0';
									tmp_str_replaced[index++] = '1';
									tmp_str_replaced[index]   = '2';
								}
								else if(tmp_str_replaced[index] == '\r'){
									tmp_str_replaced[index++] = '\\';
									tmp_str_replaced[index++] = '0';
									tmp_str_replaced[index++] = '1';
									tmp_str_replaced[index]   = '3';
								}
								index++;
							}
						}
					}

					/// "Push" the parameter on stack
					if(tmp_str_replaced != NULL){
						fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d %s@%s\n", par_index, par_index, tmp_frame, tmp_str_replaced);
						free(tmp_str_replaced);
					}
					else{
						switch(tmp->data->unit_type){
							case INTEGER:	fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d int@%d\n", par_index, par_index, *(int*)tmp->data->data); break;
							case DECIMAL:	fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d float@%a\n", par_index, par_index, *(double*)tmp->data->data); break;
							default:		fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d %s@%s\n", par_index, par_index, tmp_frame, (char*)tmp->data->data); break;
						}
					}
					par_index++;
				}

				/// "Call" the function
				if		(strcmp((char*)root->right->data->data, "inputs") == 0)		fprintf(file_descriptor, "DEFVAR TF@%%retval0\nDEFVAR TF@%%retval1\nREAD TF@retval0 string\nMOVE TF@retval1 int@1\n");
				else if	(strcmp((char*)root->right->data->data, "inputi") == 0)		fprintf(file_descriptor, "DEFVAR TF@%%retval0\nDEFVAR TF@%%retval1\nREAD TF@retval0 int\nMOVE TF@retval1 int@1\n");
				else if	(strcmp((char*)root->right->data->data, "inputf") == 0)		fprintf(file_descriptor, "DEFVAR TF@%%retval0\nDEFVAR TF@%%retval1\nREAD TF@retval0 float\nMOVE TF@retval1 int@1\n");
				else if	(strcmp((char*)root->right->data->data, "int2float") == 0)	fprintf(file_descriptor, "PUSHS TF@%%0\nINT2FLOATS\n");
				else if	(strcmp((char*)root->right->data->data, "float2int") == 0)	fprintf(file_descriptor, "PUSHS TF@%%0\nFLOAT2INTS\n");
				else if	(strcmp((char*)root->right->data->data, "len") == 0)			fprintf(file_descriptor, "DEFVAR TF@%%retval0\nSTRLEN TF@%%retval0 TF@%%0\n");
				else if	(strcmp((char*)root->right->data->data, "ord") == 0)			fprintf(file_descriptor, "PUSHS TF@%%0\nPUSHS TF@%%1\nSTR2INTS\n");
				else if	(strcmp((char*)root->right->data->data, "chr") == 0)			fprintf(file_descriptor, "PUSHS TF@%%0\nINT2CHARS\n");
				else fprintf(file_descriptor, "CALL %s\n", (char*)root->right->data->data);

				/// "Claim" returned values from function
				retval_move_reverse(root->left, file_descriptor);

				/// Reset static variable in function
				retval_move_reverse(NULL, NULL);


				/// Get rid of temrporary variables
				fprintf(file_descriptor, "CREATEFRAME\n");
			}
		}
	}
	else{
		/// "Push" children onto stack
		if(root->right != NULL && root->left != NULL){
			if((on_stack&0xf0) != 0xf0){
				switch(root->left->data->unit_type){
					case INTEGER:{
						fprintf(file_descriptor, "PUSHS int@%d\n", *((int*)root->left->data->data));
						break;
					}
					case DECIMAL:{
						fprintf(file_descriptor, "PUSHS float@%a\n", *((double*)root->left->data->data));
						break;
					}
					case IDENTIFICATOR:{
						char l_frame[3] = {0,0,0};
						s_find(var_stack, l_frame, root->left->data->data);
						fprintf(file_descriptor, "PUSHS %s@%s\n", l_frame, (char*)root->left->data->data);
						break;
					}
				}
				on_stack |= 0xf0;
			}
			if((on_stack&0x0f) != 0x0f){
				switch(root->right->data->unit_type){
					case INTEGER:{
						fprintf(file_descriptor, "PUSHS int@%d\n", *((int*)root->right->data->data));
						break;
					}
					case DECIMAL:{
						fprintf(file_descriptor, "PUSHS float@%a\n", *((double*)root->right->data->data));
						break;
					}
					case IDENTIFICATOR:{
						char r_frame[3] = "GF";
						s_find(var_stack, r_frame, root->right->data->data);
						fprintf(file_descriptor, "PUSHS %s@%s\n", r_frame, (char*)root->right->data->data);
						break;
					}
				}
				on_stack |= 0x0f;
			}
		}
		else{
			switch(root->data->unit_type){
				case INTEGER:{
					fprintf(file_descriptor, "PUSHS int@%d\n", *(int*)root->data->data);
					break;
				}
				case DECIMAL:{
					fprintf(file_descriptor, "PUSHS float@%a\n", *(double*)root->data->data);
					break;
				}
				case STRING:{
					/// Allocate enough space for replacing every character (worst case scenario)
					char* tmp_str_replaced = malloc((root->data->data_size+1)*4);
					if(tmp_str_replaced == NULL){
						fprintf(stderr, "Buffer allocation failed.(Code generation - replacing whitespace characters)\n");
						return;
					}
					int index = 0;

					/// Replace all whitespace characters with their decimal representation
					char* tmp_lex_str = (char*)root->data->data;
					while((tmp_str_replaced[index] = *(tmp_lex_str++)) != '\0'){
						if(tmp_str_replaced[index] == ' '){
							tmp_str_replaced[index++] = '\\';
							tmp_str_replaced[index++] = '0';
							tmp_str_replaced[index++] = '3';
							tmp_str_replaced[index]   = '2';
						}
						else if(tmp_str_replaced[index] == '\n'){
							tmp_str_replaced[index++] = '\\';
							tmp_str_replaced[index++] = '0';
							tmp_str_replaced[index++] = '1';
							tmp_str_replaced[index]   = '0';
						}
						else if(tmp_str_replaced[index] == '\t'){
							tmp_str_replaced[index++] = '\\';
							tmp_str_replaced[index++] = '0';
							tmp_str_replaced[index++] = '0';
							tmp_str_replaced[index]   = '9';
						}
						else if(tmp_str_replaced[index] == '\v'){
							tmp_str_replaced[index++] = '\\';
							tmp_str_replaced[index++] = '0';
							tmp_str_replaced[index++] = '1';
							tmp_str_replaced[index]   = '1';
						}
						else if(tmp_str_replaced[index] == '\f'){
							tmp_str_replaced[index++] = '\\';
							tmp_str_replaced[index++] = '0';
							tmp_str_replaced[index++] = '1';
							tmp_str_replaced[index]   = '2';
						}
						else if(tmp_str_replaced[index] == '\r'){
							tmp_str_replaced[index++] = '\\';
							tmp_str_replaced[index++] = '0';
							tmp_str_replaced[index++] = '1';
							tmp_str_replaced[index]   = '3';
						}
						index++;
					}
					fprintf(file_descriptor, "PUSHS string@%s\n", tmp_str_replaced);
					free(tmp_str_replaced);
					break;
				}
				case IDENTIFICATOR:{
					char tmp_frame[3] = {0,0,0};
					s_find(var_stack, tmp_frame, (char*)root->data->data);
					fprintf(file_descriptor, "PUSHS %s@%s\n", tmp_frame, (char*)root->data->data);
					break;
				}
			}
		}
	}
	
	/// "Preform" operation on the stack
	if(root->data->unit_type == OPERATOR){
		switch(((char*)root->data->data)[0]){
			case '*': fprintf(file_descriptor, "MULS\n"); break;
			case '/':{
				if(	root->right->data->unit_type == INTEGER ||
					root->left->data->unit_type == INTEGER  ||
					(sl_search(sl, root->right->data) != NULL && sl_search(sl, root->right->data)->id->type == INTEGER) || 
					(sl_search(sl, root->left->data) != NULL && sl_search(sl, root->left->data)->id->type == INTEGER))
					 fprintf(file_descriptor, "IDIVS\n");
				else fprintf(file_descriptor, "DIVS\n");
				break;
			}
			case '+':{
				if(	root->right->data->unit_type == STRING ||
					root->left->data->unit_type == STRING  ||
					(sl_search(sl, root->right->data) != NULL && sl_search(sl, root->right->data)->id->type == STRING) || 
					(sl_search(sl, root->left->data) != NULL && sl_search(sl, root->left->data)->id->type == STRING))
					 fprintf(file_descriptor, "DEFVAR TF@%%concat_tmp0\nDEFVAR TF@%%concat_tmp1\nPOPS TF@%%concat_tmp1\nPOPS TF@%%concat_tmp0\nCONCAT TF@%%concat_tmp0 TF@%%concat_tmp0 TF@%%concat_tmp1\nPUSHS TF@%%concat_tmp0\nCREATEFRAME\n");
				else fprintf(file_descriptor, "ADDS\n");
				break;
			}
			case '-': fprintf(file_descriptor, "SUBS\n"); break;
			case '<':{
				if(((char*)root->data->data)[1] == '=')
					 fprintf(file_descriptor, "GTS\nNOTS\n");
				else fprintf(file_descriptor, "LTS\n");
				break;
			}
			case '>':{
				if(((char*)root->data->data)[1] == '=')
					 fprintf(file_descriptor, "LTS\nNOTS\n");
				else fprintf(file_descriptor, "GTS\n");
				break;
			}
			case '=':{
				if(root->type != ASSIGNMENT && ((char*)root->data->data)[1] == '=') fprintf(file_descriptor, "EQS\n");
				break;
			}

			case '!':{
				if(root->type != ASSIGNMENT && ((char*)root->data->data)[1] == '=') fprintf(file_descriptor, "EQS\nNOTS\n");
				break;
			}
		}
	}
}

void for_unpack(d_node* root, FILE* file_descriptor, sym_list* sl){
	/// Check function arguments
	if(root == NULL || file_descriptor == NULL || label_stack == NULL || var_stack == NULL) return;

	/// Set up resources
	static int for_count = 0;
	//char tmp_frame[3] = "GF";
	//if(in_function) tmp_frame[0] = 'L';

	/// Write initialization
	root->left->right->type = ASSIGNMENT;
	expr_unpack(root->left->right, file_descriptor, sl);

	/// REWORK frames
	/// Write label, condition and conditional jump
	fprintf(file_descriptor, "DEFVAR LF@%%cmpf%d\nLABEL for_%d\n", for_count, for_count);
	expr_unpack(root->left->left->right, file_descriptor, sl);
	fprintf(file_descriptor, "POPS LF@%%cmpf%d\nJUMPIFNEQ for_%d_end LF@%%cmpf%d bool@true\n", for_count, for_count, for_count);

	/// Push label onto stack
	char tmp_var[20] = {0};
	int tmp_size = sprintf(tmp_var, "for_%d_end", for_count);
	s_push(&label_stack, tmp_var, tmp_size);

	/// Also to cut corners, push the ending expression onto label stack by pointer conversion
	root->left->left->left->right->type = ASSIGNMENT;
	s_push(&label_stack, (char*)root->left->left->left->right, -1);

	for_count++;
}

void if_unpack(d_node* root, FILE* file_descriptor, sym_list* sl){
	/// Check function arguments
	if(root == NULL || file_descriptor == NULL || label_stack == NULL || var_stack == NULL) return;

	/// Set up resources
	static int if_count = 0;
	char str[20] = {0};
	//char* tmp_lex_data = root->data->data;
	size_t tmp_lex_size = root->data->data_size;

	/// "Define" 'cmp' variable
	fprintf(file_descriptor, "DEFVAR TF@%%if_cmp%d\n", if_count);

	/// "Compute" condition
	expr_unpack(root->right, file_descriptor, sl);
	fprintf(file_descriptor, "POPS TF@%%if_cmp%d\n", if_count);

	/// Write conditional jump
	fprintf(file_descriptor, "JUMPIFNEQ else_%d TF@%%if_cmp%d bool@true\n", if_count, if_count);

	/// Push labels onto stack
	tmp_lex_size = sprintf(str, "if_%d_end", if_count);
	s_push(&label_stack, str, tmp_lex_size);
	tmp_lex_size = sprintf(str, "else_%d", if_count);
	s_push(&label_stack, str, tmp_lex_size);

	if_count++;
}

void func_unpack(d_node* root, FILE* file_descriptor){
	/// Check function arguments
	if(root == NULL || file_descriptor == NULL) return;

	/// 'Set up' function label and frames
	fprintf(file_descriptor, "LABEL %s\n%sPUSHFRAME\nCREATEFRAME\n", (char*)root->left->data->data, (strcmp((char*)root->left->data->data, "main") == 0)?("CREATEFRAME\n"):(""));

	/// 'Set up' parameters and return values
	int p_index = 0; // Parameter index
	int r_index = 0; // Return index
	for(Par* tmp = (Par*)root->left->left; tmp != NULL; tmp = tmp->next){
		/// Define parameter
		fprintf(file_descriptor, "DEFVAR LF@%s\n", (char*)tmp->name->data);

		/// Push parameter name onto 'var_stack'
		char* tmp_var = malloc(tmp->name->data_size+3);
		if(tmp_var == NULL) return;
		int tmp_var_len = sprintf(tmp_var, "LF%s", (char*)tmp->name->data);
		s_push(&var_stack, tmp_var, tmp_var_len);
		free(tmp_var);

		/// 'Set' the parameter
		fprintf(file_descriptor, "MOVE LF@%s LF@%%%d\n", (char*)tmp->name->data, p_index++);
	}
	for(Ret* tmp = (Ret*)root->left->right; tmp != NULL; tmp = tmp->next){
		/// Define retval
		fprintf(file_descriptor, "DEFVAR LF@%%retval%d\n", r_index);

		/// Push parameter name onto 'var_stack'
		char tmp_var[30] = {0};
		int tmp_var_len = sprintf(tmp_var, "LF@%%retval%d", r_index);
		s_push(&var_stack, tmp_var, tmp_var_len);

		/// 'Set' the parameter
		fprintf(file_descriptor, "MOVE LF@%%retval%d nil@nil\n", r_index);
		r_index++;
	}
}

void code_gen(d_node* root, FILE* file_descriptor, sym_list* sl){
	/// Check function arguments
	if(root == NULL || file_descriptor == NULL) return;

	/// Set up resources
	static char code_beginning = 1; // boolean substitute

	/// Write code header
	/// REWORK dynamic exit code
	if(code_beginning){
		label_stack = stack_init();
		var_stack = stack_init();
		fprintf(file_descriptor, ".IFJcode20\nCALL main\nEXIT int@0\n");
		code_beginning = 0;
	}

	/// Generate 'ifjcode20' code structures
	switch(root->data->unit_type){
		/// if, for, func, return
		case KEYWORD:{
			if(strcmp((char*)root->data->data, "if") == 0){
				if_unpack(root, file_descriptor, sl);
			}
			else if(strcmp((char*)root->data->data, "for") == 0){
				for_unpack(root, file_descriptor, sl);
			}
			else if(strcmp((char*)root->data->data, "func") == 0){
				in_function = true;
				func_unpack(root, file_descriptor);
			}
			else if(strcmp((char*)root->data->data, "return") == 0){
				/// "Push" retvals onto stack
				int ret_index = 0;
				for(d_node* tmp = root->left; tmp != NULL; tmp = tmp->left){
					expr_unpack(tmp->right, file_descriptor, sl);
					fprintf(file_descriptor, "POPS LF@%%retval%d\n", ret_index++);
				}

				/// "Pop" current frame stack
				fprintf(file_descriptor, "POPFRAME\nRETURN\n");
			}
			break;
		}

		/// =, :=, }
		case OPERATOR:{
			if(((char*)root->data->data)[0] == '}'){
				/// Check stack for labels and finish the "structures"
				if(root->right != NULL){
					if(root->right->data->unit_type == KEYWORD){
						/// Pop out else label and end label
						stack_t* else_label = s_pop(&label_stack);
						stack_t* end_label = s_pop(&label_stack);

						/// "Jump" to the end of if statement and "set up" else label
						fprintf(file_descriptor, "JUMP %s\nLABEL %s\n", end_label->str, else_label->str);

						/// Push back the end label
						s_push(&label_stack, end_label->str, end_label->str_len);
						element_free(else_label);
						element_free(end_label);
					}
					else{
						in_function = false;
						fprintf(file_descriptor, "RETURN\n");
						break;
					}
				}
				else{
					if(label_stack->str != NULL){
						stack_t* expr_or_if = s_pop(&label_stack);
						if(expr_or_if->str_len == -1){
							expr_unpack((d_node*)expr_or_if->str, file_descriptor, sl);
						}
						else{
							fprintf(file_descriptor, "LABEL %s\n", expr_or_if->str);
							/// If label is 'else'; checking 2nd char as it is unique
							if(expr_or_if->str[1] != 'l'){
								element_free(expr_or_if);
								break;
							}
						}
						element_free(expr_or_if);
						expr_or_if = s_pop(&label_stack);
						if(strstr(expr_or_if->str, "for") != NULL){
							char* tmp_pos = (strstr(expr_or_if->str, "_end"));
							*tmp_pos = '\0';
							fprintf(file_descriptor, "JUMP %s\n", expr_or_if->str);
							*tmp_pos = '_';
						}
						fprintf(file_descriptor, "LABEL %s\n", expr_or_if->str);
						element_free(expr_or_if);
					}
				}
			}
			else expr_unpack(root, file_descriptor, sl);
			break;
		}

		/// Function call
		case IDENTIFICATOR:{
			/// "Push" all parameters on stack 
			int par_index = 0;
			for(d_node* tmp = root->left; tmp != NULL; tmp = tmp->left){
				/// Find frame scope for parameter
				char* tmp_str_replaced = NULL;
				char tmp_frame[7] = {0,0,0,0,0,0,0};
				s_find(var_stack, tmp_frame, (char*)tmp->data->data);

				if(tmp_frame[0] == '\0'){
					/// Parameter is not a variable
					if(tmp->data->unit_type == STRING){
						tmp_frame[0] = 's';
						tmp_frame[1] = 't';
						tmp_frame[2] = 'r';
						tmp_frame[3] = 'i';
						tmp_frame[4] = 'n';
						tmp_frame[5] = 'g';
						tmp_frame[6] = '\0';
						
						/// Allocate enough space for replacing every character (worst case scenario)
						tmp_str_replaced = malloc((tmp->data->data_size+1)*4);
						if(tmp_str_replaced == NULL){
							fprintf(stderr, "Buffer allocation failed.(Code generation - replacing whitespace characters)\n");
							return;
						}
						int index = 0;

						/// Replace all whitespace characters with their decimal representation
						char* tmp_lex_str = (char*)tmp->data->data;
						while((tmp_str_replaced[index] = *(tmp_lex_str++)) != '\0'){
							if(tmp_str_replaced[index] == ' '){
								tmp_str_replaced[index++] = '\\';
								tmp_str_replaced[index++] = '0';
								tmp_str_replaced[index++] = '3';
								tmp_str_replaced[index]   = '2';
							}
							else if(tmp_str_replaced[index] == '\n'){
								tmp_str_replaced[index++] = '\\';
								tmp_str_replaced[index++] = '0';
								tmp_str_replaced[index++] = '1';
								tmp_str_replaced[index]   = '0';
							}
							else if(tmp_str_replaced[index] == '\t'){
								tmp_str_replaced[index++] = '\\';
								tmp_str_replaced[index++] = '0';
								tmp_str_replaced[index++] = '0';
								tmp_str_replaced[index]   = '9';
							}
							else if(tmp_str_replaced[index] == '\v'){
								tmp_str_replaced[index++] = '\\';
								tmp_str_replaced[index++] = '0';
								tmp_str_replaced[index++] = '1';
								tmp_str_replaced[index]   = '1';
							}
							else if(tmp_str_replaced[index] == '\f'){
								tmp_str_replaced[index++] = '\\';
								tmp_str_replaced[index++] = '0';
								tmp_str_replaced[index++] = '1';
								tmp_str_replaced[index]   = '2';
							}
							else if(tmp_str_replaced[index] == '\r'){
								tmp_str_replaced[index++] = '\\';
								tmp_str_replaced[index++] = '0';
								tmp_str_replaced[index++] = '1';
								tmp_str_replaced[index]   = '3';
							}
							index++;
						}
					}
				}

				/// "Push" the parameter on stack
				if(tmp_str_replaced != NULL){
					fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d %s@%s\n", par_index, par_index, tmp_frame, tmp_str_replaced);
					free(tmp_str_replaced);
				}
				else{
					switch(tmp->data->unit_type){
						case INTEGER:	fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d int@%d\n", par_index, par_index, *(int*)tmp->data->data); break;
						case DECIMAL:	fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d float@%a\n", par_index, par_index, *(double*)tmp->data->data); break;
						default:		fprintf(file_descriptor, "DEFVAR TF@%%%d\nMOVE TF@%%%d %s@%s\n", par_index, par_index, tmp_frame, (char*)tmp->data->data); break;
					}
				}
				par_index++;
			}

			/// "Call" the function
			if		(strcmp((char*)root->data->data, "print") == 0){
				for(int i = 0; i < par_index; i++){
					fprintf(file_descriptor, "WRITE TF@%%%d\n", i);
				}
			}
			else if	(strcmp((char*)root->data->data, "inputs") == 0)		fprintf(file_descriptor, "DEFVAR TF@%%retval0\nREAD TF@retval0 string\n");
			else if	(strcmp((char*)root->data->data, "inputi") == 0)		fprintf(file_descriptor, "DEFVAR TF@%%retval0\nREAD TF@retval0 int\n");
			else if	(strcmp((char*)root->data->data, "inputf") == 0)		fprintf(file_descriptor, "DEFVAR TF@%%retval0\nREAD TF@retval0 float\n");
			else if	(strcmp((char*)root->data->data, "int2float") == 0)	fprintf(file_descriptor, "PUSHS TF@%%0\nINT2FLOATS\nPOPS TF@%%0\n");
			else if	(strcmp((char*)root->data->data, "float2int") == 0)	fprintf(file_descriptor, "PUSHS TF@%%0\nFLOAT2INTS\nPOPS TF@%%0\n");
			else if	(strcmp((char*)root->data->data, "len") == 0)			fprintf(file_descriptor, "DEFVAR TF%%tmp0\nSTRLEN TF%%tmp0 tf%%0\n");
			else if	(strcmp((char*)root->data->data, "ord") == 0)			fprintf(file_descriptor, "PUSHS TF@%%0\nPUSHS TF@%%1\nSTR2INTS\nPOPS TF@%%0\n");
			else if	(strcmp((char*)root->data->data, "chr") == 0)			fprintf(file_descriptor, "PUSHS TF@%%0\nINT2CHARS\nPOPS TF@%%0\n");
			else fprintf(file_descriptor, "CALL %s\n", (char*)root->data->data);

			/// Get rid of temrporary variables
			fprintf(file_descriptor, "CREATEFRAME\n");


			break;
		}
	}
}
