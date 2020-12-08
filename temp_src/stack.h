/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Author:
* Fabo Matus	xfabom01
**/

#ifndef STACK_H
#define STACK_H

typedef struct linked_list_as_stack{
	char* str;
	int str_len;
	struct linked_list_as_stack* next;
} stack_t;

stack_t* stack_init();
void 	 s_push(stack_t** stack, char* Str, int length);
stack_t* s_pop(stack_t** stack);
void 	 s_find(stack_t* stack, char* frame_buffer, char* str); // Special function used only in "variable buffer" (special case of this stack)
void	 stack_free(stack_t* stack);
void	 element_free(stack_t* element);

#endif // STACK_H