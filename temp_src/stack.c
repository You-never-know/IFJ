/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Author:
* Fabo Matus	xfabom01
**/

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>


stack_t* stack_init(){
	stack_t* tmp = malloc(sizeof(stack_t));
	if(tmp == NULL) return NULL;
	tmp->str = NULL;
	tmp->str_len = 0;
	tmp->next = NULL;
	return tmp;
}

void s_push(stack_t** stack, char* Str, int length){
	/// Check function arguments
	if(stack == NULL || Str == NULL || length == 0) return;

	/// Find out if 'Str' is a string or expression
	if(length != -1){
		/// Check if first element is empty
		if((*stack)->str == NULL){
			(*stack)->str_len = length;

			/// Allocate enough space for the string
			(*stack)->str = malloc(length+1);
			if((*stack)->str == NULL) return;

			/// Copy the string
			while((*(*stack)->str++ = *Str++) != '\0');

			/// Return pointer to the beginning
			(*stack)->str -= length+1;
		}
		else{
			/// Create new stack element
			stack_t* tmp = malloc(sizeof(stack_t));
			if(tmp == NULL) return;
			tmp->str_len = length;
			tmp->str = malloc(length+1);
			if(tmp == NULL){
				free(tmp);
				return;
			}
			while((*tmp->str++ = *Str++) != '\0');
			tmp->str -= length+1;

			/// Insert the element in the first place
			tmp->next = *stack;

			/// Rewrite 'stack' variable
			*stack = tmp;
		}
	}
	else{
		/// Check if first element is empty
		if((*stack)->str == NULL){
			/// Assign the expression
			(*stack)->str_len = length;
			(*stack)->str = Str;
		}
		else{
			/// Create new stack element
			stack_t* tmp = malloc(sizeof(stack_t));
			if(tmp == NULL) return;

			/// Assign the expression
			tmp->str_len = length;
			tmp->str = Str;

			/// Link previous elements
			tmp->next = *stack;

			/// Rewrite 'stack' variable
			*stack = tmp;
		}
	}
}

stack_t* s_pop(stack_t** stack){
	/// Check function argument
	if(*stack == NULL) return NULL;

	/// Store current first element
	stack_t* tmp = *stack;

	/// Replace the first element
	if((*stack)->next != NULL) (*stack) = (*stack)->next;
	else{
		(*stack) = stack_init();
		if(*stack == NULL) return NULL;
	}

	/// Return popped element
	return tmp;
}

void s_find(stack_t* stack, char* frame_buffer, char* str){
	/// Check function arguments
	if(stack == NULL || stack->str == NULL || frame_buffer == NULL || str == NULL) return;

	/// Loop through elements & find wanted string
	for(stack_t* tmp = stack; tmp != NULL; tmp = tmp->next){
		/// Insert found variable frame
		if(strcmp(&tmp->str[2], str) == 0){
			frame_buffer[0] = tmp->str[0];
			frame_buffer[1] = tmp->str[1];
			frame_buffer[2] = '\0';
			return;
		}
	}
}

void stack_free(stack_t* stack){
	/// Free all elements
	stack_t* tmp_next;
	for(stack_t* tmp = stack; tmp != NULL; tmp = tmp_next){
		tmp_next = tmp->next;
		if(tmp->str != NULL && tmp->str_len != -1) free(tmp->str);
		free(tmp);
	}
}

void element_free(stack_t* element){
	/// Check function argument
	if(element == NULL) return;

	/// Free element
	if(element->str != NULL){
		if(element->str_len != -1){
			free(element->str);
			element->str = NULL;
		}
	}
	free(element);

	/// Null given pointer
	element = NULL;
}

/*
void main(){
	stack_t* heyy = stack_init();
	s_push(&heyy, "ASS\n", 4);
	s_push(&heyy, "MY ", 3);
	s_push(&heyy, "FUCK ", 5);

	for(int i = 0; i < 3; i++){
		stack_t* tmp = s_pop(&heyy);
		printf("%s", tmp->str);
		element_free(tmp);
	}
	stack_free(heyy);
}*/