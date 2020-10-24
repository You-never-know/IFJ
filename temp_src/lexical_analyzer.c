#include "lexical_analyzer.h"

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
	c |= 1<<5;
	return (c >= 'a' && c <= 'z');
}

bool isOperator(const char c){
	const char operators[] = "+-*/%=&|^<>!~(){}[].,";
	for(unsigned char i = 0; i < 11; i++){
		if(c == operators[i]) return true;
	}
	return false;
}

/*
 * Checks if given char is an operator that can be part of multi-char operator
 */
bool isMultiOperator(const char c){
	const char operators[] = "+-*%=&|^<>!"; // '/' is missing intentionally
	for(unsigned char i = 0; i < 11; i++){
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

		case 1: if(!isMultiOperator(c)) return true;
				/// Check all operators that can't be doubled (++, &&, ||, ...)
				if(	op_buff[0] == '*' || op_buff[0] == '%' ||
					op_buff[0] == '^' || op_buff[0] == '!')
											return (c == '=');
				else if(op_buff[0] == '~')	return false;

				/// Rest can be doubled
				else if(op_buff[0] == c)	return true;

				/// Special cases
				else if(c == '=')	return (op_buff[0] != '>' && op_buff[0] != '<');
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
						(memcmp(lex->data, "for", 3ul) == 0));
				break;

		case 4:	return ((memcmp(lex->data, "else", 4ul) == 0) ||
						(memcmp(lex->data, "func", 4ul) == 0));
				break;

		case 6:	return ((memcmp(lex->data, "string", 6ul) == 0) ||
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
bool spaceRealloc(lex_unit_t* lex, allocated_size){
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

lex_unit_t* Analyze(FILE* file_descriptor){
	/// Check function argument
	if(file_descriptor == NULL) return NULL;

	/// Set up resources for state machine
	enum states{START, OPER_OUT, ID_OUT, KW_OUT, INT_OUT, DEC_OUT, STR_OUT, 		// I/O states
				COMMENT_START, COMMENT_END, L_COMMENT, ML_COMMENT,					// Comment states (L = Line; ML = Multi Line)
				WORD_LOAD, INT_LOAD, DEC_LOAD, EXP_DEC_LOAD, STR_LOAD, ML_STR_LOAD,	// Loading states
				OPER_CHECK, ESCAPE_CHAR, EXP_DEC_LOAD_CHECK,						// Check states
				OPER_ERROR, ID_ERROR, INT_ERROR, DEC_ERROR, STR_ERROR, INVALID};	// Error states
	unsigned char state = START;	// Current state
	int c = 0; 						// Current character

	/// Create new lexical unit
	lex_unit_t* lexeme = malloc(sizeof(lex_unit_t));
	if(lexeme == NULL) return NULL;
	LexUnitCtor(lexeme);
	lexeme->data = malloc(CHUNK_SIZE);
	if(lexeme->data == NULL) return NULL;
	size_t allocated_size = CHUNK_SIZE;

	/// Loop through each character in given file
		// REWORK fix 'EOF' char
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
									c == '~')		state = OPER_OUT;
							else if(isMultiOperator(c))	state = OPER_CHECK;
							else					state = INVALID;
						}
						break;

			case COMMENT_START:	if(c == '*') 		state = ML_COMMENT;
								else if(c == '/')	state = L_COMMENT;
								else if(c == '=' || c == '_' || c == '"' || c == '`' ||
										isWhiteSpace(c) || isLetter(c) || isNumber(c))
									 state = OPER_OUT;
								else state = OPER_ERROR;
								break;

			case L_COMMENT:	if(c == '\n') state = START;
							break;

			case ML_COMMENT:	if(c == '*') state = COMMENT_END;
								break;

			case COMMENT_END:	if(c == '/')		state = START;
								else if(c != '*')	state = ML_COMMENT;
								break;

			case OPER_CHECK:	if(	c == '_' || c == '"' || c == '`' ||
									isWhiteSpace(c) || isLetter(c) || isNumber(c))
									state = OPER_OUT;
								else if(!isOperValid(lexeme, c)) state = OPER_ERROR;
								break;

			case WORD_LOAD:	if(isWhiteSpace(c) || isOperator(c)){
								if(isKeyword(lexeme))	state = KW_OUT;
								else					state = ID_OUT;
							}
							break;

			case INT_LOAD:	if(!isNumber(c)){
								if(c == '.')		state = DEC_LOAD;
								else if(c == 'e')	state = EXP_DEC_LOAD_CHECK;
								else if(isWhiteSpace(c) || isOperator(c)) state = INT_OUT;
								else				state = INT_ERROR;
							}
							break;

			case DEC_LOAD:	if(!isNumber(c)){
								if(c == 'e')	state = EXP_DEC_LOAD_CHECK;
								else if(isWhiteSpace(c) || isOperator(c)) state = DEC_OUT;
								else 			state = DEC_ERROR;
							}
							break;

			case EXP_DEC_LOAD_CHECK:	if(c == '-' || isNumber(c)) state = EXP_DEC_LOAD;
										else state = DEC_ERROR;
										break;

			case EXP_DEC_LOAD:	if(!isNumber){
									if(isWhiteSpace(c) || isOperator(c)) state = DEC_OUT;
									else state = DEC_ERROR;
								}
								break;

			case ML_STR_LOAD:	if(c == '`') state = STR_OUT;
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
								else if(c == '\\' || c == '"') state = STR_LOAD;
								else state = STR_ERROR;
								break;

			default: fprintf(stderr, "<Lexical Analyzer> Wrong state: %d\n", state);
		}

		/// (State Machine) state behaviour
		switch(state){
			/// Starting state, Escaping char state and Comment states do nothing

			/// Save to buffer without checking
			case OPER_CHECK: case INT_LOAD: case DEC_LOAD:
			case EXP_DEC_LOAD_CHECK: case EXP_DEC_LOAD: case WORD_LOAD:
				if(spaceRealloc(lexeme, allocated_size)) return NULL;

				/// Save 'c' in data buffer
				lexeme->data[lexeme->data_size++] = c;
				break;

			case ML_STR_LOAD:	if(c != '`'){
									if(spaceRealloc(lexeme, allocated_size))
										return NULL;
					
									/// Save 'c' in data buffer
									lexeme->data[lexeme->data_size++] = c;
								}
								break;

			case STR_LOAD:	if(lexeme->size != 0 || c != '"'){
								if(spaceRealloc(lexeme, allocated_size))
									return NULL;
				
								/// Save 'c' in data buffer
								lexeme->data[lexeme->data_size++] = c;
							}
							break;

			/// Outputs
			case ID_OUT:	lexeme->unit_type = IDENTIFICATOR;
							lexeme->data = realloc(lexeme->data, lexeme->data_size); // 'data_size' should be always smaller than allocated size
							ungetc(file_descriptor, c);
							return lexeme;
							break;

			case KW_OUT:	lexeme->unit_type = KEYWORD;
							lexeme->data = realloc(lexeme->data, lexeme->data_size); // 'data_size' should be always smaller than allocated size
							ungetc(file_descriptor, c);
							return lexeme;
							break;

			case INT_OUT:	lexeme->unit_type = INTEGER;
							lexeme->data[lexeme->data_size] = '\0';
							size_t tmp = atoi((char*)lexeme->data);
							lexeme->data = realloc(lexeme->data, sizeof(size_t));
							*((size_t*)lexeme->data) = tmp;
							ungetc(file_descriptor, c);
							return lexeme;
							break;

			case DEC_OUT:	lexeme->unit_type = DECIMAL;
							lexeme->data[lexeme->data_size] = '\0';
							double tmp = atof((char*)lexeme->data);
							lexeme->data = realloc(lexeme->data, sizeof(size_t));
							*((double*)lexeme->data) = tmp;
							ungetc(file_descriptor, c);
							return lexeme;
							break;

			case OPER_OUT:	lexeme->unit_type = OPERATOR;
							lexeme->data = realloc(lexeme->data, lexeme->data_size); // 'data_size' should be always smaller than allocated size
							if(!isOperator(c)) ungetc(file_descriptor, c);
							return lexeme;
							break;

			case STR_OUT:	lexeme->unit_type = STRING;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
							lexeme->data[lexeme->data_size] = '\0';
							return lexeme;
							break;

			/// Errors
			case OPER_ERROR:	if(spaceRealloc(lexeme, allocated_size)) return NULL;

								/// Save 'c' in data buffer
								lexeme->data[lexeme->data_size++] = c;
								lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
								lexeme->unit_type = OPERATOR_ERR;
								break;

			case ID_ERROR:	if(spaceRealloc(lexeme, allocated_size)) return NULL;

							/// Save 'c' in data buffer
							lexeme->data[lexeme->data_size++] = c;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
							lexeme->unit_type = ID_ERR;
							break;

			case INT_ERROR:	if(spaceRealloc(lexeme, allocated_size)) return NULL;

							/// Save 'c' in data buffer
							lexeme->data[lexeme->data_size++] = c;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
							lexeme->unit_type = INT_ERR;
							break;
							
			case DEC_ERROR:	if(spaceRealloc(lexeme, allocated_size)) return NULL;

							/// Save 'c' in data buffer
							lexeme->data[lexeme->data_size++] = c;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
							lexeme->unit_type = DEC_ERR;
							break;
							
			case STR_ERROR:	if(spaceRealloc(lexeme, allocated_size)) return NULL;

							/// Save 'c' in data buffer
							lexeme->data[lexeme->data_size++] = c;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
							lexeme->unit_type = STR_ERR;
							break;
							
			case INVALID:	if(spaceRealloc(lexeme, allocated_size)) return NULL;

							/// Save 'c' in data buffer
							lexeme->data[lexeme->data_size++] = c;
							lexeme->data = realloc(lexeme->data, lexeme->data_size+1); // 'data_size' should be always lower than allocated size
							lexeme->unit_type = ERROR;
							break;
							
		}
	}
}