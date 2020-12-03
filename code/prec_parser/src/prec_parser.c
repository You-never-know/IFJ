/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
**/

#include "structs.h"
#include "d_tree.h"
#include <stdbool.h>
#include "lexical_analyzer.h"
#include "lex_list.h"
#include "prec_parser.h"
#include "prec_table.h"

// stacks needed for this program
lex_list * stack = NULL;
lex_list * help = NULL;



// get the type of the most top operand
int get_the_top_operand() {

	for (ll_elem_ptr tmp = stack->first; tmp != NULL; tmp = tmp->r) {
		if (tmp->ll_data->type != HANDLE && tmp->ll_data->type != E) {
			return tmp->ll_data->type;
		}
	}

	return ERR;

}

// add hande after the last operand
void add_handle() {

	d_node * tmp = NULL;

	while (1) {

		tmp = ll_return_first_data(stack);
		if (tmp->type == E) {
			ll_insert_first(help, tmp); // add it to the help stack
			ll_del_first(stack); // delete it from the stack
		}
		else {
			d_node * node = d_node_create(NULL, NULL, HANDLE); // create handle
			ll_insert_first(stack, node); // add handle to the stack

			while (ll_get_length(help) != 0) { // add back the items from the help stack
				node = ll_return_first_data(help);
				ll_insert_first(stack, node); // add it to the stack
				ll_del_first(help); // delete it from the help stack
			}
			return;
		}
	}
}

// test if we are finished
bool test_end(int input) {

	return (get_the_top_operand() == DOLLAR && input == DOLLAR);
}


void clean () {
	ll_dissolve(help);
	ll_dissolve(stack);
}


bool match() {

	d_node * tmp = ll_return_first_data(help);

	switch (tmp->type) {
	 	case  I: // E -> i
			ll_del_first(help);
			if (ll_get_length(help) != 0) {
				return false;
			}
			tmp->type = E;
			ll_insert_first(stack, tmp);
			return true;


		case E: // E -> E operator E
			if (ll_get_length(help) == 3) {
				d_node * E1 = tmp;
				ll_del_first(help);
				d_node * OP = ll_return_first_data(help);
				ll_del_first(help);
				d_node * E2 = ll_return_first_data(help);
				ll_del_first(help);

				if (E2->type == E && (OP->type == PLUS_MINUS || OP ->type == MUL_DIV || OP ->type == COMPARISON)) {
					d_node_insert_left(OP, E1);
					d_node_insert_right(OP, E2);
					OP->type = E;
					ll_insert_first(stack, OP);
					return true;
				}

				delete_tree(E1);
				delete_tree(OP);
				delete_tree(E2);
				return false;
			}
			else {
				return false;
			}


		case L_BRACKET:

			if (ll_get_length(help) == 3) { // E -> (E)
				d_node * LB = tmp;
				ll_del_first(help);
				d_node * E_m = ll_return_first_data(help);
				ll_del_first(help);
				d_node * RB = ll_return_first_data(help);
				ll_del_first(help);

				if (RB->type == R_BRACKET && E_m -> type == E) {
					delete_tree(LB);
					delete_tree(RB);
					ll_insert_first(stack,E_m);
					return true;
				}

				delete_tree(LB);
				delete_tree(E_m);
				delete_tree(RB);
				return false;
			}
			else {
				return false;
			}

		case F:
			if (ll_get_length(help) == 4) { // E -> f(E)
				d_node * fun = tmp; // f
				ll_del_first(help);
				d_node * LB = ll_return_first_data(help); // (
				ll_del_first(help);
				d_node * par = ll_return_first_data(help); // E
				ll_del_first(help);
				d_node * RB = ll_return_first_data(help); // )
				ll_del_first(help);

				if (LB->type == L_BRACKET && RB->type == R_BRACKET && par->type == E) {
					delete_tree(LB);
					delete_tree(RB);
					d_node_insert_left(fun, par);
					ll_insert_first(stack, fun);
					return true;
				}

				delete_tree(fun);
				delete_tree(LB);
				delete_tree(RB);
				delete_tree(par);
				return false;
			}else if (ll_get_length(help) == 3) { // E -> f()
				d_node* func = tmp; // f
				ll_del_first(help);
				d_node* LB = ll_return_first_data(help); // (
				ll_del_first(help);
				d_node* RB = ll_return_first_data(help); // )
				ll_del_first(help);

				if (LB->type == L_BRACKET && RB->type == R_BRACKET) {
					delete_tree(LB);
					delete_tree(RB);
					ll_insert_first(stack, func);
					return true;
				}

				delete_tree(func);
				delete_tree(LB);
				delete_tree(RB);
				return false;
			}
			else { // E -> f(E,E, ... , E)

				int length = ll_get_length(help);

				if (length < 6) { // the minumum is f(E,E)
					return false;
				}

				ll_del_first(help); // we already have f

				d_node * function = tmp;
				d_node * under_node = function;
				d_node * node = ll_return_first_data(help);
				ll_del_first(help);

				if (node->type != L_BRACKET) {
					delete_tree(function);
					delete_tree(node);
					return false;
				}
				delete_tree(node); // we don't need a (

				length = length-2; // -f -(
				bool need_comma_bracket = false;
				bool correct = false;
				for (int i = 0; i<length; i++) {

					node = ll_return_first_data(help);
					ll_del_first(help);

					if (need_comma_bracket == false) { // check if E
						if (node->type == E) {
							d_node_insert_left(under_node, node); // add parameter to the left
							under_node = node; // add another parameter to the next node
							need_comma_bracket = true;
							correct = false;
							continue;
						}
						delete_tree(function);
						delete_tree(node);
						return false;
					}
					else { // check for ) or ,
						if (node -> type == COMMA) {
							delete_tree(node);
							need_comma_bracket = false;
							correct = false;
							continue;
						}
						else if (node -> type == R_BRACKET) {
							delete_tree(node);
							need_comma_bracket = false;
							correct = true;
							continue;
						}
						else {
							delete_tree(function);
							delete_tree(node);
							return false;
						}
					}

				} // end of for
				if (correct == true) {
					function->type = E;
					ll_insert_first(stack, function);
					return true;
				}
				delete_tree(function);
				return false;
			}
			break;

		default:
			return false;
	}

	return false;
}



// match the rule and change the stack acordingly
bool match_rule() {

	bool cont = true;
	d_node * tmp = NULL;

	while (cont) {
		tmp = ll_return_first_data(stack);
		if (tmp->type == DOLLAR) { // we try to match empty stack
			fprintf(stderr, "match_rule failed\n");
			return false;
		}
		else if (tmp->type == HANDLE) { // the help stack is ready
			ll_del_first(stack); // remove the handle
			if (match() == false) {
				return false;
			}
			return true;
		}
		else { // get the data from the stack to the help one
			ll_insert_first(help, tmp); // add to the help stack
			ll_del_first(stack);
		}

	}

	return false;

}


/* Make the precedence syntactic analysis
 * 'token' the current token in the file
 * 'root' the node where the tree structure comes
 * 'start' tokens from the buffer start of expresion in end out
 * return true if happend corectly, false if not
 */
bool Parse_expresion(lex_unit_t * token, d_node * root, token_list ** start, sym_tab * fun_tab) {

	// check parameters
	if (token == NULL || root == NULL || start == NULL) {
		return false;
	}

	if ((*start) == NULL) {
		return false;
	}

	// init stack
	stack = ll_init();
	help = ll_init();

	// put $ to the stack
	d_node * node = d_node_create(NULL, NULL, DOLLAR); // create node with $
	ll_insert_first(stack, node); // put it into stack

	// help vaiables
	bool finished = false; // end the cycle analyse is complete
	bool get_token = true; // get another token or not from the file
	int type = merge_event(token, fun_tab); // get the first type

	while (!finished) {
		if (type != ERR) {
			type = merge_event(token, fun_tab);
		}
		if (type == ERR) {
			get_token = false; // dont read anything from the input
			type = DOLLAR; // set it as the input
		}
printf("TOKEN SIZE %ld\n",(token)->data_size );

		if (get_token) { // we create only new nodes
			node = d_node_create(NULL, token, type); // create node
		}

		int next_move = 42;
		next_move = prec_event(get_the_top_operand(), type);
		printf("GTO: %d\n",	get_the_top_operand());
		printf("NEXT MOVE: %d\n",	next_move );

		switch (next_move) {

			case 0: 
				ll_insert_first(stack, node);
				get_token = true;
			
				break;

			case -1:
				add_handle(); 
				ll_insert_first(stack, node);
				get_token = true;
				break;

			case 1: // prec_table > try to match any rule
		
				if (match_rule() == true) {
					get_token = false;
				}
				else { // rule not found
					clean();
					root->right = NULL;
					return false;
				}

				if (test_end(type)) { // test if we are finished
					finished = true;
					get_token = false;
					break;
				}
				break;

			default:
		
				clean();
	
				root->right = NULL;
				return false;

		}


		if (get_token) { // get the next token if needed
			if ((*start) == NULL) {
				clean();
				return false;
			}
			token = (*start)->unit;
			*start = (*start)->next;
			printf("TOKEN PRINT %s; (%d) %d\n", (char*)(token)->data,*((int*)(token)->data), (token)->unit_type);

		}
	}

	// test if everything happend correctly
	if (stack->length == 2 && help->length == 0) {
			d_node * top = ll_return_first_data(stack);
			ll_del_first(stack);

			if (top == NULL) {
				clean();
				root->right = NULL;
				return false;
			}
			else if (top->type == E) {
				root->right = top;
				clean();
				return true;
			}
		}

		clean();
		root->right = NULL;
		return false;

}
