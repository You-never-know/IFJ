#include "lexical_analyzer.h"
#include <assert.h>
#include <ctype.h>
#include <unistd.h>


unsigned WORD_COUNT=0;

void Error(const char *msg){
	fprintf(stderr,"%s\n",msg);
	exit(1); 
}
bool Reason_to_break(const int c){
	if(c==EOF)return true;
	if(c=='"')return true;
	if(isOperator(c)&&c!='.')return true;
	const char special_c[] = "\n/";
	for(unsigned char i = 0; i < 2; i++){
		if(c == special_c[i]) return true;
	}
	return false;
}
void Operators(int chr,FILE*go_file){
	int twinkle_little_star;//Operators
	if(isOperator(chr)){
		do{
			twinkle_little_star=chr;
			fprintf(stdout,"%c",chr);
			chr=fgetc(go_file);
		}while(isOperator(chr)&&isOperator(twinkle_little_star));
		WORD_COUNT++;
		if(!isOperator(chr))ungetc(chr,go_file);
	}
	
}
void String(int chr,FILE*go_file){
	if(chr=='"'){// checking for string
		do{
			fprintf(stdout,"%c",chr);
			chr = fgetc(go_file);
		}while(chr != EOF && chr !='"');
			WORD_COUNT++;
		if(chr!=EOF)fprintf(stdout,"%c",chr);
	}
}
void Comments(int chr,FILE*go_file){
	fprintf(stdout,"%c",chr);
	chr=fgetc(go_file);
	if(chr=='/'){// checking for single line comment
		do{
			fprintf(stdout,"%c",chr);
			chr = fgetc(go_file);
		}while(chr != EOF && chr !='\n');
		if(chr=='\n')WORD_COUNT++;
		if(chr=='\n')fprintf(stdout,"%c",chr);
	} 
else if(chr=='*'){// checking for multi line comment
		do{	
			do{
				if(chr=='\n')WORD_COUNT++;
				fprintf(stdout,"%c",chr);
				chr = fgetc(go_file);
			}while(chr != EOF && chr !='*');
			if(chr != EOF)fprintf(stdout,"%c",chr);
			if(chr != EOF)chr = fgetc(go_file);
		}while(chr!=EOF && chr!='/'); //checking for end of comment
	if(chr!=EOF)ungetc(chr,go_file);
	}
	else WORD_COUNT++;
}
void Lex_count(FILE* go_file){
	if(go_file == NULL)Error("missing file");
	WORD_COUNT = 0;
	int chr=0;
	do{
		do{
			while(isspace(chr = fgetc(go_file)) && (!(Reason_to_break(chr))))fprintf(stdout,"%c",chr);//white space is not lex
			if(chr == EOF || Reason_to_break(chr))break;
			fprintf(stdout,"%c",chr);
			WORD_COUNT++;               
			while(!(isspace(chr = fgetc(go_file))) && (!(Reason_to_break(chr))))fprintf(stdout,"%c",chr);//printing numbers and words
			if(!(Reason_to_break(chr)))fprintf(stdout,"%c",chr); 
		}while(!(Reason_to_break(chr)));

		switch(chr)
		{
			case '/': Comments(chr,go_file);
				break;

			case EOF: 
				break;

			case '\n': WORD_COUNT++,fprintf(stdout,"%c",chr);
				break;

			case '"': String(chr,go_file);
				break;

			default: Operators(chr,go_file);
			break;
		}	
	}while(chr!=EOF);
	return;
}


void Prints_lex(lex_unit_t* First,unsigned number_of_units){
	unsigned counter=0;
	if(First == NULL){assert(counter==number_of_units);return;}
	lex_unit_t* tmp = First;
	fprintf(stdout,"~~~~~~~~\n");
	fprintf(stdout,"Lexemes:\n");
	fprintf(stdout,"-+-------------------\n");
	while(tmp!=NULL){
		counter++;
		fprintf(stdout," | unit_type: ");
		switch(tmp->unit_type){
			case ERROR:			fprintf(stdout, "Error\n"); break;
			case OPERATOR:		fprintf(stdout, "Operator\n"); break;
			case IDENTIFICATOR:	fprintf(stdout, "Identificator\n"); break;
			case KEYWORD:		fprintf(stdout, "Keyword\n"); break;
			case INTEGER:		fprintf(stdout, "Integer\n"); break;
			case DECIMAL:		fprintf(stdout, "Decimal\n"); break;
			case STRING:		fprintf(stdout, "String\n"); break;
			case OPERATOR_ERR:	fprintf(stdout, "Operator Error\n"); break;
			case ID_ERR:		fprintf(stdout, "Identificator Error\n"); break;
			case INT_ERR:		fprintf(stdout, "Integer Error\n"); break;
			case DEC_ERR:		fprintf(stdout, "Decimal Error\n"); break;
			case STR_ERR:		fprintf(stdout, "String Error\n"); break;
			default:			fprintf(stdout, "%d\n", tmp->unit_type); break;
		}
		fprintf(stdout," | data_size: %ld\n",tmp->data_size);
		fprintf(stdout," | data: ");
		switch(tmp->unit_type){
			case INTEGER:		fprintf(stdout, "%ld\n", *((size_t*)tmp->data)); break;
			case DECIMAL:		fprintf(stdout, "%f\n", *((double*)tmp->data)); break;
			case STRING:		fprintf(stdout, "\"%s\"\n", (char*)tmp->data); break;
			default:			fprintf(stdout, "%s\n", (char*)tmp->data); break;
		}
		tmp=tmp->next;
		fprintf(stdout," +-------------------\n");
	}
	assert(counter==number_of_units);
}

FILE* Creating_file(const char *filename,const char *text){

	if(filename==NULL)Error("Could not open file");
	if(text==NULL)Error("Could not open file");
	//only accepts .go files
	if(strcmp(strrchr(filename,'.'),".go")!=0)Error("Wrong file");
	//trying to open file w+
	FILE* go_file=fopen(filename,"w+");


	if(go_file==NULL)Error("Could not open file"); 
	if(fputs(text,go_file)<0)Error("Could not write to file"); 

    rewind(go_file);//to reset the pointer to the start of the file
	 
	fprintf(stdout,"text in file:\n");
    Lex_count(go_file);
    fprintf(stdout,"\n");

    rewind(go_file);

	return go_file;
}

lex_unit_t* Loading_lex_units(FILE * go_file){

	if(go_file==NULL)Error("file gone wild");

	lex_unit_t *act = malloc(sizeof(lex_unit_t));
	if(act==NULL) Error("Lexical unit allocation failed");
	LexUnitCtor(act);
	lex_unit_t *first=act; // first ptr of lex_units
	lex_unit_t* last_act = NULL;
	while(Analyze(go_file, act) != NULL){ // loading units
		act->next = malloc(sizeof(lex_unit_t));
		if(act->next == NULL) Error("Lexical unit allocation failed");
		LexUnitCtor(act->next);
		last_act = act;
		act=act->next;
	}
	if(act == first && act->data_size == 0)first = NULL;
	LexUnitDelete(act);
	if(last_act != NULL) last_act->next = NULL;
	return first;
}

void Free_Lex_Units(lex_unit_t* first){
	/// Check function argument
	if(first == NULL) return;

	/// Loop through & free each unit in 'lex_unit_t' linked list
	lex_unit_t* tmp;
	lex_unit_t* tmp_next;
	for(tmp = first; tmp != NULL; tmp = tmp_next){
		tmp_next = tmp->next;
		LexUnitDelete(tmp);
	}
}

int main()
{
	//------------//
	/* TEST01_ID  */
	/* testing ID */
	//------------//

	fprintf(stdout,"\n======TEST01_ID======\n");
	FILE * go_file=Creating_file("test01.go","casa casca + 44 8454343 ***a=4*2");
	lex_unit_t* lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	
  
	//------------//
	/* TEST02_ID  */
	/* testing ID */
	//------------//

	fprintf(stdout,"\n======TEST02_ID======\n");
	go_file=Creating_file("t.go","_id   ___420var\n ____ int return 420blazeitvar");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//------------------------//
	/*       TEST03_NL        */
	/* testing Newline lexeme */
	//------------------------//

	fprintf(stdout,"\n======TEST03_NL======\n");
	go_file=Creating_file("t.go","\n\ncomment//comemnt\n>>=\n_I_have_MASSIVE\npackage\n42\n420.69\n\"A cat ate my homework, so I ate the cat :3\"\n::\nmeet@midnight_and_sacrifice\n80085_are_best\n276492E=EA_SPORTS_ITS_IN_A_GAME\n\"When I ate the cat, its owner caught me and ate me >:(\t\n");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//------------------------//
	/*       TEST04_NL        */
	/* testing Newline lexeme */
	//------------------------//

	fprintf(stdout,"\n======TEST04_NL======\n");
	go_file=Creating_file("t.go","= \n a \n if \n 8 \n 6.9 \n \"x\" \n :: \n 8d \n 4.2o \n \"\\k\" \n");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST05_HEX        */
	/* testing Hexadecimal char */
	//--------------------------//

	fprintf(stdout,"\n======TEST05_HEX======\n");
	go_file=Creating_file("t.go","\"I wonder what this is: \\x32\"\n\"And this?: \\x48\"\n\"And this should be an error: \\xNOPE :)\"");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST06_INT        */
	/* testing Integer numbers  */
	//--------------------------//

	fprintf(stdout,"\n======TEST06_INT======\n");
	go_file=Creating_file("t.go","10 0 01 0001 0x20 0xx20 0FA 0b10 0bb10 0b12 0o12 0o19");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST07_INT        */
	/* testing exp numbers  	*/
	//--------------------------//

	fprintf(stdout,"\n======TEST06_INT======\n");
	go_file=Creating_file("t.go","12e+4 1e-4 8E-E 18E+4sss");
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,4); //LEX COUNT DOSENT WORK HERE!!!!!!!!!!
	Free_Lex_Units(lex_first);
	fclose(go_file);


	return 0;

}
