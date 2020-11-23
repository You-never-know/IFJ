
/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Drengubiak Vladimír	xdreng01
* Fabo Matúš			xfabom01
**/

#include "lexical_analyzer.h"
#include <assert.h>
#include <ctype.h>
#include <unistd.h>

typedef struct lex_u_list //linked list for test 
{
	lex_unit_t * unit;
	struct lex_u_list *next;
}lex_list;

unsigned WORD_COUNT=0;

void Error(const char *msg){
	fprintf(stderr,"%s\n",msg);
	exit(1); 
}
bool Reason_to_break(const int c){
	if(c==EOF)return true;
	if(c=='"')return true;
	if(c==';')return true;
	if(isOperator(c))return true;
	const char special_c[] = "\n/";
	for(unsigned char i = 0; i < 2; i++){
		if(c == special_c[i]) return true;
	}
	return false;
}
bool Exception(int chr){
	const char exception[] = ";()[]{}.";
	for(unsigned char i = 0; i < 8; i++){
		if(chr == exception[i]) return true;
	}
	return false;

}
void Operators(int chr,FILE*go_file){
	int twinkle_little_star;//Operators
	if(isOperator(chr)&&(!Exception(chr))){
		do{
			twinkle_little_star=chr;
			fprintf(stdout,"%c",chr);
			chr=fgetc(go_file);
		}while(isOperator(chr)&&isOperator(twinkle_little_star));
		WORD_COUNT++;
		if(!isOperator(chr))ungetc(chr,go_file);
	}
	else WORD_COUNT++,fprintf(stdout,"%c",chr);
	
}
void String(int chr,FILE*go_file){
	if(chr=='"'){// checking for string
		int tmp;
		do{
			tmp=chr;
			fprintf(stdout,"%c",chr);
			if(chr=='\n')WORD_COUNT++;
			chr = fgetc(go_file);
			if(tmp=='\\'&&chr=='"')fprintf(stdout,"%c",chr),chr = fgetc(go_file);
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
	else WORD_COUNT++,ungetc(chr,go_file);
}
void Lex_count(FILE* go_file){
	if(go_file == NULL)Error("missing file");
	WORD_COUNT = 0;
	int chr=0;
	int tmp;
	do{
		do{
			while(isspace(chr = fgetc(go_file)) && (!(Reason_to_break(chr))))fprintf(stdout,"%c",chr);//white space is not lex
			if(chr == EOF || Reason_to_break(chr))break;
			fprintf(stdout,"%c",chr);
			tmp=chr;
			WORD_COUNT++;               
			while(((!(isspace(chr = fgetc(go_file)))) && (!(Reason_to_break(chr)))) || (((!(isspace(tmp))) && chr=='"'))  || ((isNumber(tmp)) && chr=='.') || ((isMultiOperator(tmp) && isMultiOperator(chr))))tmp=chr,fprintf(stdout,"%c",chr);//printing numbers and words
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


void Prints_lex(lex_list* First,int number_of_units){//-1 ignores assert 
	int counter=0;
	if(First == NULL){if(number_of_units!=-1)assert(counter==number_of_units);return;}
	lex_list* tmp = First;
	fprintf(stdout,"~~~~~~~~\n");
	fprintf(stdout,"Lexemes:\n");
	fprintf(stdout,"-+-------------------\n");
	while(tmp!=NULL){
		counter++;
		fprintf(stdout," | unit_type: ");
		switch(tmp->unit->unit_type){
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
			default:			fprintf(stdout, "%d\n", tmp->unit->unit_type); break;
		}
		fprintf(stdout," | data_size: %ld\n",tmp->unit->data_size);
		fprintf(stdout," | data: ");
		switch(tmp->unit->unit_type){
			case INTEGER:		fprintf(stdout, "%ld\n", *((size_t*)tmp->unit->data)); break;
			case DECIMAL:		fprintf(stdout, "%f\n", *((double*)tmp->unit->data)); break;
			case STRING:		fprintf(stdout, "\"%s\"\n", (char*)tmp->unit->data); break;
			default:			fprintf(stdout, "%s\n", (char*)tmp->unit->data); break;
		}
		tmp=tmp->next;
		fprintf(stdout," +-------------------\n");
	}
	fprintf(stdout,"NUMBER OF REAL TOKENS      : %d \n",counter);
	fprintf(stdout,"NUMBER OF LEX_COUNT TOKENS : %d \n",number_of_units);
	if(number_of_units!=-1)assert(counter==number_of_units);

}

FILE* Creating_file(bool wanna_count,bool open_only,const char *filename,const char *text){

	if(filename==NULL)Error("Could not open file");
	if(!open_only)if(text==NULL)Error("Could not open file");
	//only accepts .go files
	if(strcmp(strrchr(filename,'.'),".go")!=0)Error("Wrong file");
	//trying to open file w+
	FILE* go_file=(!open_only)?fopen(filename,"w+"):fopen(filename,"r");

	if(go_file==NULL)Error("Could not open file"); 
	if(!open_only)if(fputs(text,go_file)<0)Error("Could not write to file");

    rewind(go_file);//to reset the pointer to the start of the file
	int chr=0;
	fprintf(stdout,"text in file:\n");
    if(wanna_count==true)Lex_count(go_file);
    else{
    	do{
			fprintf(stdout,"%c",chr);
			chr = fgetc(go_file);
		}while(chr != EOF);
    }
    fprintf(stdout,"\n");

    rewind(go_file);

	return go_file;
}

lex_list* Loading_lex_units(FILE * go_file){
	if(go_file==NULL)Error("file gone wild");
	lex_list *act = malloc(sizeof(lex_list));
	if(act==NULL)Error("list gone wild");
	act->unit = malloc(sizeof(lex_unit_t));
	if(act->unit==NULL)Error("Lexical unit allocation failed");
	LexUnitCtor(act->unit);
	lex_list *first=act; // first ptr of lex_units
	lex_list * last_act = NULL;
	while(Analyze(go_file, act->unit) != NULL){ // loading units
		act->next = malloc(sizeof(lex_list));
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

void Free_Lex_Units(lex_list* first){
	/// Check function argument
	if(first == NULL) return;

	/// Loop through & free each unit in 'lex_unit_t' linked list
	lex_list* tmp;
	lex_list* tmp_next;
	for(tmp = first; tmp != NULL; tmp = tmp_next){
		tmp_next = tmp->next;
		LexUnitDelete(tmp->unit);
		free(tmp);
	}
}

int main()
{
	//------------//
	/* TEST01_ID  */
	/* testing ID */
	//------------//

	fprintf(stdout,"\n======TEST01_ID======\n");
	FILE * go_file=Creating_file(1,0,"test01.go","casa casca + 44 8454343 ***a=4*2 ");
	lex_list* lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//------------//
	/* TEST02_ID  */
	/* testing ID */
	//------------//

	fprintf(stdout,"\n======TEST02_ID======\n");
	go_file=Creating_file(1,0,"t.go","_id   ___420var\n ____ int return 420blazeitvar");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//------------------------//
	/*       TEST03_NL        */
	/* testing Newline lexeme */
	//------------------------//

	fprintf(stdout,"\n======TEST03_NL======\n");
	go_file=Creating_file(1,0,"t.go","\n\ncomment//comemnt\n>>=\n_I_have_MASSIVE\npackage\n42\n420.69\n\"A cat ate my homework, so I ate the cat :3\"\n::\nmeet@midnight_and_sacrifice\n80085_are_best\n276492E=EA_SPORTS_ITS_IN_A_GAME\n\"When I ate the cat, its owner caught me and ate me >:(\t\n");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//------------------------//
	/*       TEST04_NL        */
	/* testing Newline lexeme */
	//------------------------//

	fprintf(stdout,"\n======TEST04_NL======\n");
	go_file=Creating_file(1,0,"t.go","= \n a \n if \n 8 \n 6.9 \n \"x\" \n :: \n 8d \n 4.2o \n \"\\k\" \n");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST05_HEX        */
	/* testing Hexadecimal char */
	//--------------------------//

	fprintf(stdout,"\n======TEST05_HEX======\n");
	go_file=Creating_file(1,0,"t.go","\"I wonder what this is: \\x32\"\n\"And this?: \\x48\"\n\"And this should be an error: \\xNOPE :)\"");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST06_INT        */
	/* testing Integer numbers  */
	//--------------------------//

	fprintf(stdout,"\n======TEST06_INT======\n");
	go_file=Creating_file(1,0,"t.go","10 0 01 0001 0x20 0xx20 0FA 0b10 0bb10 0b12 0o12 0o19");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST07_EXP        */
	/* testing exp numbers 		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST07_EXP======\n");
	go_file=Creating_file(1,0,"t.go","12e+4 1e-4 8E-E 18E+4sss");
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,-1); //LEX COUNT DOSENT WORK HERE!!!!!!!!!!
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST08_RANDOM     */
	/* testing random  			*/
	//--------------------------//

	fprintf(stdout,"\n======TEST08_RANDOM======\n");  
	go_file=Creating_file(1,0,"t.go","INT S = CCC + asfasf acascasxxx xvbsefwdv qe q3r23r24twwrsd er 24r 24t wrg we 2e t2rgsdf 32 t24t 2ef e r2t 24t 2ef 2ed e qsf dge arng MNWFNWEKJfnAJLDBGJWBefj b2 24b fjlwebf andflk baojbt jowbefj DSJF BWEW 4asfafs  5155414{ }");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST09_RANDOM     */
	/* testing random  			*/
	//--------------------------//

	fprintf(stdout,"\n======TEST09_RANDOM======\n");  
	go_file=Creating_file(1,0,"t.go"," \" INT S = CCC + asfasf acascasxxx xvbsefwdv \\n \" qe q3r23r24twwrsd er \" 24r24t wrg we 2e t2rgsdf \"32 t24t 2ef e r2 \"t 24t 2ef 2ed e q \"sf dge arng MNWFNWEKJfnA\"JLDBGJWBefj b2 24b fjlwebf an \" dflk ba \" ojbt jowbefj DSJF BWEW 4asfafs  5155414");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST10_GO_CODE    */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST10_GO_CODE======\n");  
	go_file=Creating_file(1,0,"t.go","package main \n import \"fmt\" \n func main()\n { \n rows := 9 \n space := rows / 2 \n num := 1 \n}");
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST11_GO_CODE1   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST11_GO_CODE======\n");  
	go_file=Creating_file(1,1,"go_code0.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST12_GO_CODE2   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST12_GO_CODE======\n");  
	go_file=Creating_file(1,1,"go_code1.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST13_GO_CODE3   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST13_GO_CODE======\n");  
	go_file=Creating_file(1,1,"example1.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST14_GO_CODE4   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST14_GO_CODE======\n");  
	go_file=Creating_file(1,1,"example2.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST15_GO_CODE5   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST15_GO_CODE======\n");  
	go_file=Creating_file(1,1,"example3.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST16_GO_CODE6   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST16_GO_CODE======\n");  
	go_file=Creating_file(1,1,"example4.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST17_GO_CODE7   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST17_GO_CODE======\n");  
	go_file=Creating_file(1,1,"example5.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//--------------------------//
	/*        TEST18_GO_CODE8   */
	/* testing go_code  		*/
	//--------------------------//

	fprintf(stdout,"\n======TEST18_GO_CODE======\n");  
	go_file=Creating_file(1,1,"example6.go",NULL);// from file
	lex_first=Loading_lex_units(go_file); 
	Prints_lex(lex_first,WORD_COUNT); 
	Free_Lex_Units(lex_first);
	fclose(go_file);




	return 0;

}
