#include "code_gen.h"
#include "d_tree.h"
#include "code_gen.c"
#include "prec_table.h"
#include "symtable.h"
#include "stack.h"

int main(){
	lex_unit_t par_1 		= {IDENTIFICATOR, "nope", 4, NULL};
	lex_unit_t par_0 		= {IDENTIFICATOR, "Heyy", 4, NULL};
	lex_unit_t f_name_lex 	= {IDENTIFICATOR, "nice", 4, NULL};
	lex_unit_t f_lex 		= {KEYWORD, 	  "func", 4, NULL};
	Ret   ret2 = {0, NULL};
	Ret   ret1 = {0, &ret2};
	Ret   ret0 = {0, &ret1};
	Par   par1 = {&par_1, 0, NULL};
	Par   par0 = {&par_0, 0, &par1};
	d_node f_name = {DOLLAR, &f_name_lex, (d_node*)&par0, (d_node*)&ret0};
	d_node f_def = {DOLLAR, &f_lex, &f_name, NULL};

	int x = 0;
	int y = 12;
	int z = 69;
	lex_unit_t	retval_2	= {INTEGER,	&z,	sizeof(int), NULL};
	lex_unit_t	retval_1	= {INTEGER,	&y,	sizeof(int), NULL};
	lex_unit_t	retval_0	= {INTEGER,	&x,	sizeof(int), NULL};
	lex_unit_t	ret			= {KEYWORD,	"return",	6, NULL};
	d_node		retval2	= {DOLLAR,	&retval_2,	NULL,  NULL};
	d_node		retval1	= {DOLLAR,	&retval_1,	NULL,  NULL};
	d_node		retval0	= {DOLLAR,	&retval_0,	NULL,  NULL};
	d_node		dol2	= {DOLLAR,	NULL,	NULL,  &retval2};
	d_node		dol1	= {DOLLAR,	NULL,	&dol2, &retval1};
	d_node		dol0	= {DOLLAR,	NULL,	&dol1, &retval0};
	d_node		end		= {DOLLAR,	&ret,	&dol0, NULL};


	code_gen(&f_def, stdout);
	code_gen(&end, stdout);
	return 0;
}

/*			============FUNCTION DEFINITION=============
	lex_unit_t par_1 		= {IDENTIFICATOR, "nope", 4, NULL};
	lex_unit_t par_0 		= {IDENTIFICATOR, "Heyy", 4, NULL};
	lex_unit_t f_name_lex 	= {IDENTIFICATOR, "nice", 4, NULL};
	lex_unit_t f_lex 		= {KEYWORD, 	  "func", 4, NULL};
	Ret   ret2 = {0, NULL};
	Ret   ret1 = {0, &ret2};
	Ret   ret0 = {0, &ret1};
	Par   par1 = {&par_1, 0, NULL};
	Par   par0 = {&par_0, 0, &par1};
	d_node f_name = {DOLLAR, &f_name_lex, (d_node*)&par0, (d_node*)&ret0};
	d_node f_def = {DOLLAR, &f_lex, &f_name, NULL};

	int x = 0;
	int y = 12;
	int z = 69;
	lex_unit_t	retval_2	= {INTEGER,	&z,	sizeof(int), NULL};
	lex_unit_t	retval_1	= {INTEGER,	&y,	sizeof(int), NULL};
	lex_unit_t	retval_0	= {INTEGER,	&x,	sizeof(int), NULL};
	lex_unit_t	ret			= {KEYWORD,	"return",	6, NULL};
	d_node		retval2	= {DOLLAR,	&retval_2,	NULL,  NULL};
	d_node		retval1	= {DOLLAR,	&retval_1,	NULL,  NULL};
	d_node		retval0	= {DOLLAR,	&retval_0,	NULL,  NULL};
	d_node		dol2	= {DOLLAR,	NULL,	NULL,  &retval2};
	d_node		dol1	= {DOLLAR,	NULL,	&dol2, &retval1};
	d_node		dol0	= {DOLLAR,	NULL,	&dol1, &retval0};
	d_node		end		= {DOLLAR,	&ret,	&dol0, NULL};*/

/*			============FOR=============
	int x = 0;
	int y = 10;
	lex_unit_t	lex_for		= {KEYWORD,			"for",	3, NULL};
	lex_unit_t	ass_init	= {OPERATOR,		":=",	2, NULL};
	lex_unit_t	comp		= {OPERATOR,		"<",	1, NULL};
	lex_unit_t	plus		= {OPERATOR,		"+",	1, NULL};
	lex_unit_t	ass_end 	= {OPERATOR,		"=",	1, NULL};
	lex_unit_t	id			= {IDENTIFICATOR,	"i",	1, NULL};
	lex_unit_t	id0			= {INTEGER,			&x,		sizeof(int), NULL};
	lex_unit_t	id10		= {INTEGER,			&y,		sizeof(int), NULL};
	d_node		id_10 	= {I, 			&id10, 		NULL, 		NULL};
	d_node		id_comp	= {I, 			&id, 		NULL, 		NULL};
	d_node		end_expr= {PLUS_MINUS,	&plus,		&id_comp,	&id_10};
	d_node		id_0 	= {I, 			&id0, 		NULL, 		NULL};
	d_node		id_end 	= {I, 			&id, 		NULL, 		&end_expr};
	d_node		id_init = {I, 			&id, 		NULL, 		&id_0};
	d_node		expr2 	= {ASSIGNMENT,	&ass_end, 	&id_end,	NULL};
	d_node		expr1 	= {COMPARISON,	&comp,		&id_comp, 	&id_10};
	d_node		expr0	= {ASSIGNMENT,	&ass_init, 	&id_init, 	NULL};
	d_node		dol2 	= {DOLLAR, 		NULL, 		NULL, 		&expr2};
	d_node		dol1 	= {DOLLAR, 		NULL, 		&dol2, 		&expr1};
	d_node		dol0 	= {DOLLAR, 		NULL, 		&dol1, 		&expr0};
	d_node		ass 	= {DOLLAR, 		&lex_for, 	&dol0, 		NULL};

	lex_unit_t	bracket	= {OPERATOR,	"}",		1, NULL};
	d_node		end		= {R_BRACKET,	&bracket,	NULL, NULL};*/

/*			==========FUNCTION CALL============
	int x = 10;
	lex_unit_t 	id_0 = {IDENTIFICATOR, "ID1", 3, NULL};
	lex_unit_t 	id_1 = {IDENTIFICATOR, "_Fucker", 7, NULL};
	lex_unit_t 	id_2 = {IDENTIFICATOR, "ID30", 4, NULL};
	lex_unit_t 	func_name = {IDENTIFICATOR, "func", 4, NULL};
	lex_unit_t 	par_1 = {STRING, "Hey fucker, I will fucking kill you sometime soon\n", 50, NULL};
	lex_unit_t 	par_2 = {INTEGER, &x, sizeof(int), NULL};
	lex_unit_t 	eq = {OPERATOR, "=", 1, NULL};
	d_node		id2 = {I, &id_2, NULL, NULL};
	d_node		id1 = {I, &id_1, &id2, NULL};
	d_node		id0 = {I, &id_0, &id1, NULL};
	d_node		par2 = {I, &par_2, NULL, NULL};
	d_node		par1 = {I, &par_1, &par2, NULL};
	d_node		func = {I, &func_name, &par1, NULL};
	d_node 		ass = {ASSIGNMENT, &eq, &id0, &func};*/

/*			========var := "Hey bitch\n"=========
	lex_unit_t 	oof = {STRING, "Hey bitch\n", 11, NULL};
	lex_unit_t 	lex_var = {IDENTIFICATOR, "var", 3, NULL};
	lex_unit_t 	eq = {OPERATOR, ":=", 2, NULL};
	d_node		oof_str = {0, &oof, NULL, NULL};
	d_node 		var = {I, &lex_var, NULL, &oof_str};
	d_node 		ass = {ASSIGNMENT, &eq, &var, NULL};*/

/*			==========IF==========
	int x = 10;
	int y = 20;
	lex_unit_t x_lex	= {INTEGER,			&x, sizeof(int), NULL};
	lex_unit_t y_lex	= {INTEGER,			&y, sizeof(int), NULL};
	lex_unit_t lt		= {OPERATOR,		"<",  1, NULL};
	lex_unit_t if_lex	= {KEYWORD,			"if", 2, NULL};
	d_node ten		= {DOLLAR,		&x_lex,		NULL,	NULL};
	d_node twenty	= {DOLLAR,		&y_lex,		NULL,	NULL};
	d_node expr		= {COMPARISON,	&lt, 		&ten,	&twenty};
	d_node cmp		= {DOLLAR,		&if_lex,	NULL,	&expr};
	
	lex_unit_t	bracket	= {OPERATOR,	"}",		1, NULL};
	d_node		end		= {R_BRACKET,	&bracket,	NULL, NULL};*/