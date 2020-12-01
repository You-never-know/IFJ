/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Authors:
* Fabo Matúš			xfabom01
* Drengubiak Vladimír	xdreng01
**/


#include "lexical_analyzer.h"

void Error(const char *msg){
	fprintf(stderr,"%s\n",msg);
	exit(99); 
}

bool isWhiteSpace(const char c){
	if(c < '!'){
		const char whitespace[] = " \t\n\v\f\r\0";
		for(unsigned char i = 0; i < 6; i++){
			if(c == whitespace[i]) return true;
		}
	}
	return false;
}

bool isNumber(const char c){
	return (c >= '0' && c <= '9');	
}

bool isLetter(char c){
	c |= 1<<5; // Convert letters to lowercase
	return (c >= 'a' && c <= 'z');
}

bool isOperator(const char c){
	const char operators[] = "+-*/%=&|^<>!~(){}[].,;";
	for(unsigned char i = 0; i < 22; i++){
		if(c == operators[i]) return true;
	}
	return false;
}

/*
 * Checks if given char is an operator that can be part of multi-char operator
 */
bool isMultiOperator(const char c){
	const char operators[] = "+-*%=&|^<>!:"; // '/' is missing intentionally
	for(unsigned char i = 0; i < 12; i++){
		if(c == operators[i]) return true;
	}
	return false;
}

bool isOperValid(const lex_unit_t* lex, char c){
	/// Check function argument
	if(lex == NULL || lex->data_size >= 3 || !isMultiOperator(c))
		return false;

	/// Decide if is operator valid
	const char* op_buff = lex->data;
	switch(lex->data_size){
		case 0: return isMultiOperator(c);
				break;

		case 1: if(!isMultiOperator(c)) return false;
				/// Check all operators that can't be doubled (++, &&, ||, ...)
				if(	op_buff[0] == '*' || op_buff[0] == '%' ||
					op_buff[0] == '^' || op_buff[0] == '!' ||
					op_buff[0] == ':')		return (c == '=');
				else if(op_buff[0] == '~')	return false;

				/// Rest can be doubled
				else if(op_buff[0] == c)	return true;

				/// Special cases
				else if(c == '=')	return (op_buff[0] == '>' || op_buff[0] == '<');
				else if(c == '>')	return (op_buff[0] == '-');
				else 				return false;
				break;

		case 2:	if(c == '=') return((op_buff[0] == '>' && op_buff[1] == '>') ||
									(op_buff[0] == '<' && op_buff[1] == '<'));
				return false;
				break;

		default:	return false;
	}
}

bool isKeyword(const lex_unit_t* lex){
	/// Check function argument
	if(lex == NULL || lex->data_size < 2 || lex->data_size > 7) return false;

	/// Check for keywords: if, int, for, else, func, string, return, float64, package
	switch(lex->data_size){
		case 2:	return (memcmp(lex->data, "if", 2ul) == 0);
				break;

		case 3:	return ((memcmp(lex->data, "int", 3ul) == 0) ||
						(memcmp(lex->data, "var", 3ul) == 0) || 
						(memcmp(lex->data, "for", 3ul) == 0));
				break;

		case 4:	return ((memcmp(lex->data, "else", 4ul) == 0) ||
						(memcmp(lex->data, "func", 4ul) == 0));
				break;

		case 6:	return ((memcmp(lex->data, "string", 6ul) == 0) ||
						(memcmp(lex->data, "import", 6ul) == 0) ||
						(memcmp(lex->data, "return", 6ul) == 0));
				break;

		case 7:	return ((memcmp(lex->data, "float64", 7ul) == 0) ||
						(memcmp(lex->data, "package", 7ul) == 0));
				break;
		default: return false;
	}
}

/*
 * Reallocs 'data' buffer, returns true on failure (like raising a flag)
 */
bool spaceRealloc(lex_unit_t* lex, size_t allocated_size){
	/// Check if buffer has enough size
	if(lex->data_size+1 > allocated_size){
		allocated_size += CHUNK_SIZE;
		lex_unit_t* tmp = realloc(lex->data, allocated_size);
		if(tmp == NULL){
			free(lex->data);
			lex->data = NULL;
			lex->data_size = 0;
			return true;
		}
		else lex->data = tmp;
	}
	return false;
}

bool NumSystemCheck(const char* str, const size_t str_size, const char number_base){
	/// Check function arguments
	if(str == NULL || str_size == 0 || (number_base != 2 && number_base != 8 && number_base != 16))
		return false;

	/// Loop through each char & check if its right
	for(size_t i = 0; i < str_size; i++){
		switch(number_base){
			case 2:	if(str[i] != '0' && str[i] != '1') return false;
					break;

			case 8:	if((str[i]-0x30) >= 8) return false;
					break;

			case 16:	if(!isLetter(str[i]) && (str[i]|1<<5) < 'a' && (str[i]|1<<5) > 'f')
							return false;
						break;
		}
	}
	return true;
}

token_list* Loading_lex_units(FILE * go_file){
	if(go_file==NULL)Error("file gone wild");
	token_list *act = malloc(sizeof(token_list));
	if(act==NULL)Error("list gone wild");
	act->unit = malloc(sizeof(lex_unit_t));
	if(act->unit==NULL)Error("Lexical unit allocation failed");
	LexUnitCtor(act->unit);
	token_list *first=act; // first ptr of lex_units
	token_list * last_act = NULL;
	while(Analyze(go_file, act->unit) != NULL){ // loading units
		act->next = malloc(sizeof(token_list));
		if(act->next == NULL) Error("list gone wild");
		act->next->unit =malloc(sizeof(lex_unit_t));
		if(act->next->unit == NULL) Error("Lexical unit allocation failed");
		LexUnitCtor(act->next->unit);
		last_act = act;
		act=act->next;
	}
	if(act->unit == first->unit && act->unit->data_size == 0)first->unit = NULL;
	LexUnitDelete(act->unit);
	if(last_act != NULL){
		free(last_act->next);
		last_act->next = NULL;
	}
	return first;
}

lex_unit_t* Analyze(FILE* file_descriptor, lex_unit_t* unit){
	/// Check function argument
	if(file_descriptor == NULL) return NULL;

	/// Set up resources for state machine
	enum states{START, OPER_OUT, ID_OUT, KW_OUT, INT_OUT, DEC_OUT, STR_OUT, NL_OUT,	// I/O states
				COMMENT_START, COMMENT_END, L_COMMENT, ML_COMMENT,					// Comment states (L = Line; ML = Multi Line)
				WORD_LOAD, INT_LOAD, DEC_LOAD, EXP_DEC_LOAD, STR_LOAD, ML_STR_LOAD,	// Loading states
				OPER_CHECK, ESCAPE_CHAR, EXP_DEC_LOAD_CHECK, HEX_REP_CHECK,			// Check states
				OPER_ERROR, ID_ERROR, INT_ERROR, DEC_ERROR, STR_ERROR, INVALID};	// Error states
	unsigned char state = START;	// Current state
	int c = 0; 						// Current character
	bool ml_comment_nl_flag = false;

	/// Clear given lexical unit
	LexUnitClear(unit);
	lex_unit_t* lexeme = (unit == NULL)?(LexUnitCreate()):(unit);
	if(lexeme == NULL) return NULL;
	lexeme->data = malloc(CHUNK_SIZE);
	if(lexeme->data == NULL){
		if(unit == NULL) LexUnitDelete(lexeme);
		return NULL;
	}
	size_t allocated_size = CHUNK_SIZE;

	/// Loop through each character in given file
	while(c != EOF){
		/// Get next char
		c = getc(file_descriptor);

		/// Implement a finite-state machine (See Lexical Analyzer diagram)
		/// (State Machine) state deciding logic
		switch(state){
			case START:	if(!isWhiteSpace(c)){
							if(isLetter(c) || c == '_')	state = WORD_LOAD;
							else if(isNumber(c))	state = INT_LOAD;
							else if(c == '"')		state = STR_LOAD;
							else if(c == '`')		state = ML_STR_LOAD;
							else if(c == '/')		state = COMMENT_START;
							else if(c == '(' || c == ')' ||
									c == '{' || c == '}' || 
									c == '[' || c == ']' || 
									c == '.' || c == ',' ||
									c == '~' || c == ';' )	state = OPER_OUT;
							else if(isMultiOperator(c))	state = OPER_CHECK;
							else					state = INVALID;
						}
						else if(c == '\n')			state = NL_OUT;
						break;

			case COMMENT_START:	if(c == '*') 		state = ML_COMMENT;
								else if(c == '/')	state = L_COMMENT;
								else if(c == '=' || c == '_' || c == '"' || c == '`' ||
										c == '(' || c == ')' ||
										isWhiteSpace(c) || isLetter(c) || isNumber(c)){
									((char*)lexeme->data)[lexeme->data_size++] = '/';
									state = OPER_OUT;
								}
								else{
									((char*)lexeme->data)[lexeme->data_size++] = '/';
									state = OPER_ERROR;
								}
								break;

			case L_COMMENT:	if(c == '\n') state = NL_OUT;
							break;

			case ML_COMMENT:	if(c == '*') state = COMMENT_END;
								else if(c == '\n') ml_comment_nl_flag = true;
								break;

			case COMMENT_END:	if(c == '/'){
									if(ml_comment_nl_flag)	state = NL_OUT;
									else					state = START;
								}
								else if(c != '*')	state = ML_COMMENT;
								if(c == '\n')		ml_comment_nl_flag = true;
								break;

			case OPER_CHECK:	if(!isMultiOperator(c))				state = OPER_OUT;
								else if(!isOperValid(lexeme, c))	state = OPER_ERROR;
								break;

			case WORD_LOAD:	if(isWhiteSpace(c) || isOperator(c) || c == EOF){
								if(isKeyword(lexeme))	state = KW_OUT;
								else					state = ID_OUT;
							}
							else if(!(isLetter(c) || isNumber(c) || c == '_'))
														state = ID_ERROR;
							break;

			case INT_LOAD:	if(!isNumber(c)){
								if(lexeme->data_size > 1 && (((char*)lexeme->data)[0] == '0' && isNumber(((char*)lexeme->data)[1])))
															state = INT_ERROR;
								else if(c == '.')			state = DEC_LOAD;
								else if((c|1<<5) == 'e')	state = EXP_DEC_LOAD_CHECK;
								else if(isWhiteSpace(c) || isOperator(c) || c == EOF)
															state = INT_OUT;
								else if(lexeme->data_size == 1 && ((c|1<<5) == 'b' || (c|1<<5) <= 'x' || (c|1<<5) <= 'o'))
															break;
								else						state = INT_ERROR;
							}
							break;

			case DEC_LOAD:	if(!isNumber(c)){
								if(lexeme->data_size > 1 && (((char*)lexeme->data)[0] == '0' && isNumber(((char*)lexeme->data)[1])))
															state = DEC_ERROR;
								else if((c|1<<5) == 'e')	state = EXP_DEC_LOAD_CHECK;
								else if(isWhiteSpace(c) || isOperator(c) || c == EOF)
															state = DEC_OUT;
								else 						state = DEC_ERROR;
							}
							break;

			case EXP_DEC_LOAD_CHECK:	if(c == '-' || c == '+' || isNumber(c))
												state = EXP_DEC_LOAD;
										else	state = DEC_ERROR;
										break;

			case EXP_DEC_LOAD:	if(!isNumber(c)){
									if(isWhiteSpace(c) || isOperator(c) || c == EOF)
											state = DEC_OUT;
									else	state = DEC_ERROR;
								}
								break;

			case ML_STR_LOAD:	if(c == '`')		state = STR_OUT;
								else if(c == EOF)	state = STR_ERROR;
								break;

			case STR_LOAD:	if(c == '"')		state = STR_OUT;
							else if(c == '\\')	state = ESCAPE_CHAR;
							else if(c < 32)		state = STR_ERROR;
							break;

			case ESCAPE_CHAR:	if(c == 'a'){
									c = '\a';
									state = STR_LOAD;
								}
								else if(c == 'b'){
									c = '\b';
									state = STR_LOAD;
								}
								else if(c == 'f'){
									c = '\f';
									state = STR_LOAD;
								}
								else if(c == 'n'){
									c = '\n';
									state = STR_LOAD;
								}
								else if(c == 'r'){
									c = '\r';
									state = STR_LOAD;
								}
								else if(c == 't'){
									c = '\t';
									state = STR_LOAD;
								}
								else if(c == 'v'){
									c = '\v';
									state = STR_LOAD;
								}
								else if(c == 'x'){
									state = HEX_REP_CHECK;
								}
								else if(c == '\\' || c == '"') state = STR_LOAD;
								else{
									ungetc(c, file_descriptor);
									c = '\\';
									state = STR_ERROR;
								}
								break;

			default: if(state > INVALID) fprintf(stderr, "<Lexical Analyzer> Wrong state(state decision): %d\n", state);
		}
		
		/// Handle EOF
		if(c == EOF && lexeme->data_size == 0){
			if(unit == NULL) LexUnitDelete(lexeme);
			return NULL;
		}

		/// (State Machine) state behaviour
		switch(state){
			/// Starting state, Escaping char state and Comment states do nothing

			/// Save to buffer without checking
			case OPER_CHECK: case INT_LOAD: case DEC_LOAD:
			case EXP_DEC_LOAD_CHECK: case EXP_DEC_LOAD: case WORD_LOAD:
				if(spaceRealloc(lexeme, allocated_size)){
					if(unit == NULL) LexUnitDelete(lexeme);
					return NULL;
				}

				/// Save 'c' in data buffer
				((char*)lexeme->data)[lexeme->data_size++] = c;
				break;

			case HEX_REP_CHECK:	; char hex_buff[3] = "\0\0\0"; // The ';' is there intentionally
								hex_buff[0] = getc(file_descriptor);
								hex_buff[1] = getc(file_descriptor);
								if(	(isNumber(hex_buff[0]) || (isLetter(hex_buff[0]) && (hex_buff[0]|1<<5) <= 'f')) && 
									(isNumber(hex_buff[1]) || (isLetter(hex_buff[1]) && (hex_buff[1]|1<<5) <= 'f')))
								{
									if(spaceRealloc(lexeme, allocated_size)){
										if(unit == NULL) LexUnitDelete(lexeme);
										return NULL;
									}
									((char*)lexeme->data)[lexeme->data_size++] = (char)strtol(hex_buff, NULL, 16);
									state = STR_LOAD;
								}
								else{
									ungetc(hex_buff[1], file_descriptor);
									ungetc(hex_buff[0], file_descriptor);
									ungetc('x', file_descriptor);
									ungetc('\\', file_descriptor);
									state = STR_ERROR;
								}
								break;

			case ML_STR_LOAD:	if(c != '`'){
									if(spaceRealloc(lexeme, allocated_size)){
										if(unit == NULL) LexUnitDelete(lexeme);
										return NULL;
									}
					
									/// Save 'c' in data buffer
									((char*)lexeme->data)[lexeme->data_size++] = c;
								}
								break;

			case STR_LOAD:	if(lexeme->data_size != 0 || c != '"'){
								if(spaceRealloc(lexeme, allocated_size)){
									if(unit == NULL) LexUnitDelete(lexeme);
									return NULL;
								}
				
								/// Save 'c' in data buffer
								((char*)lexeme->data)[lexeme->data_size++] = c;
							}
							break;

			/// Outputs
			case ID_OUT:	lexeme->unit_type = IDENTIFICATOR;
							((char*)lexeme->data)[lexeme->data_size] = '\0';
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always smaller than allocated size
							if(c != EOF) ungetc(c, file_descriptor);
							return lexeme;
							break;

			case KW_OUT:	lexeme->unit_type = KEYWORD;
							((char*)lexeme->data)[lexeme->data_size] = '\0';
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always smaller than allocated size
							if(c != EOF) ungetc(c, file_descriptor);
							return lexeme;
							break;

			case INT_OUT:	lexeme->unit_type = INTEGER;
							((char*)lexeme->data)[lexeme->data_size] = '\0';
							size_t tmp_int = atoi((char*)lexeme->data);
							if(lexeme->data_size > 2){
								char num_type = ((char*)lexeme->data)[1];
								bool err_flag = false;
								if((((char*)lexeme->data)[1]|1<<5) == 'b'){
									((char*)lexeme->data)[1] = '0';
									tmp_int = strtol((char*)lexeme->data, NULL, 2);
									if(!NumSystemCheck((char*)lexeme->data, lexeme->data_size, 2))
										err_flag = true;
								}
								else if((((char*)lexeme->data)[1]|1<<5) == 'o'){
									((char*)lexeme->data)[1] = '0';
									tmp_int = strtol((char*)lexeme->data, NULL, 8);
									if(!NumSystemCheck((char*)lexeme->data, lexeme->data_size, 8))
										err_flag = true;
								}
								else if((((char*)lexeme->data)[1]|1<<5) == 'x'){
									((char*)lexeme->data)[1] = '0';
									tmp_int = strtol((char*)lexeme->data, NULL, 16);
									if(!NumSystemCheck((char*)lexeme->data, lexeme->data_size, 16))
										err_flag = true;
								}

								if(err_flag){
									lexeme->unit_type = INT_ERR;
									((char*)lexeme->data)[1] = num_type;
									lexeme->data = realloc(lexeme->data, lexeme->data_size+1);
									if(c != EOF) ungetc(c, file_descriptor);
									return lexeme;
									break;
								}
							}
							lexeme->data_size = sizeof(size_t);
							lexeme->data = realloc(lexeme->data, lexeme->data_size);
							*((size_t*)lexeme->data) = tmp_int;
							if(c != EOF) ungetc(c, file_descriptor);
							return lexeme;
							break;

			case DEC_OUT:	lexeme->unit_type = DECIMAL;
							if(!isNumber(((char*)lexeme->data)[lexeme->data_size-1]))
								lexeme->unit_type = DEC_ERR; 
							((char*)lexeme->data)[lexeme->data_size] = '\0';
							double tmp_float = atof((char*)lexeme->data);
							lexeme->data_size = sizeof(double);
							lexeme->data = realloc(lexeme->data, lexeme->data_size);
							*((double*)lexeme->data) = tmp_float;
							if(c != EOF) ungetc(c, file_descriptor);
							return lexeme;
							break;

			case OPER_OUT:	if(lexeme->data_size == 0 || (((char*)lexeme->data)[0] == '/' && c == '='))
								((char*)lexeme->data)[lexeme->data_size++] = c;
							((char*)lexeme->data)[lexeme->data_size] = '\0';
							lexeme->unit_type = OPERATOR;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always smaller than allocated size
							if(!isOperator(c)) ungetc(c, file_descriptor);
							return lexeme;
							break;

			case STR_OUT:	lexeme->unit_type = STRING;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
							((char*)lexeme->data)[lexeme->data_size] = '\0';
							return lexeme;
							break;

			case NL_OUT:	lexeme->unit_type = NEWLINE;
							((char*)lexeme->data)[0] = '\n';
							lexeme->data_size = 1;
							lexeme->data = realloc(lexeme->data, 2);
							((char*)lexeme->data)[lexeme->data_size] = '\0';
							return lexeme;
							break;

			/// Errors
			case OPER_ERROR:	if(spaceRealloc(lexeme, allocated_size)){
									if(unit == NULL) LexUnitDelete(lexeme);
									return NULL;
								}
								if(isWhiteSpace(c) || isNumber(c) || isLetter(c) || c == EOF){
									lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
									((char*)lexeme->data)[lexeme->data_size] = '\0';
									lexeme->unit_type = OPERATOR_ERR;
									if(!isOperator(c)) ungetc(c, file_descriptor);
									return lexeme;
								}
								else ((char*)lexeme->data)[lexeme->data_size++] = c;
								break;

			case ID_ERROR:	if(spaceRealloc(lexeme, allocated_size)){
								if(unit == NULL) LexUnitDelete(lexeme);
								return NULL;
							}

							/// Save 'c' in data buffer
							if(isWhiteSpace(c) || isOperator(c) || c == EOF){
								lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
								((char*)lexeme->data)[lexeme->data_size] = '\0';
								lexeme->unit_type = ID_ERR;
								if(isOperator(c) || c == '"' || c == '`' || c == '\n') ungetc(c, file_descriptor);
								return lexeme;
							}
							else ((char*)lexeme->data)[lexeme->data_size++] = c;
							break;

			case INT_ERROR:	if(spaceRealloc(lexeme, allocated_size)){
								if(unit == NULL) LexUnitDelete(lexeme);
								return NULL;
							}

							if(isWhiteSpace(c) || isOperator(c) || c == EOF){
								lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
								((char*)lexeme->data)[lexeme->data_size] = '\0';
								lexeme->unit_type = INT_ERR;
								if(isOperator(c) || c == '\n') ungetc(c, file_descriptor);
								return lexeme;
							}
							else ((char*)lexeme->data)[lexeme->data_size++] = c;
							break;
							
			case DEC_ERROR:	if(spaceRealloc(lexeme, allocated_size)){
								if(unit == NULL) LexUnitDelete(lexeme);
								return NULL;
							}

							if(isWhiteSpace(c) || isOperator(c) || c == EOF){
								lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
								((char*)lexeme->data)[lexeme->data_size] = '\0';
								lexeme->unit_type = DEC_ERR;
								if(isOperator(c) || c == '\n') ungetc(c, file_descriptor);
								return lexeme;
							}
							else ((char*)lexeme->data)[lexeme->data_size++] = c;
							break;
							
			case STR_ERROR:	if(spaceRealloc(lexeme, allocated_size)){
								if(unit == NULL) LexUnitDelete(lexeme);
								return NULL;
							}

							if(c == '\n' || c == '"' || c == EOF){
								lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
								((char*)lexeme->data)[lexeme->data_size] = '\0';
								lexeme->unit_type = STR_ERR;
								if(c == '\n') ungetc(c, file_descriptor);
								return lexeme;
							}
							else ((char*)lexeme->data)[lexeme->data_size++] = c;
							break;
							
			case INVALID:	if(spaceRealloc(lexeme, allocated_size)){
								if(unit == NULL) LexUnitDelete(lexeme);
								return NULL;
							}

							if(isWhiteSpace(c) || isOperator(c) || c == EOF){
								lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
								((char*)lexeme->data)[lexeme->data_size] = '\0';
								lexeme->unit_type = ERROR;
								if(c == '\n') ungetc(c, file_descriptor);
								return lexeme;
							}
							else ((char*)lexeme->data)[lexeme->data_size++] = c;
							break;

			default: if(state > INVALID) fprintf(stderr, "<Lexical Analyzer> Wrong state(state behaviour): %d\n", state);
		}
	}

	/// This shouldn't even happen
	if(unit == NULL) LexUnitDelete(lexeme);
	return NULL;
}
