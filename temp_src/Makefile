#
# Project:
# Implementace pøekladaèe imperativního jazyka IFJ20
#
# Author:
# Drengubiak Vladimír	xdreng01
# Daniel Marek          xmarek72
# Kateøina Nepra¹ová	xnepra01

CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra
COMPILATOR = gcc $(CFLAGS) -o $@


all:  translate 

translate: translate.o lexical_analyzer.o structs.o symtable.o prec_parser.o d_tree.o sym_list.o lex_list.o prec_table.o create_tables.o syntactic_analyzer.o sem_analysis.o code_gen.o stack.o
	$(COMPILATOR) $^

translate.o: translate.c create_tables.h prec_parser.h d_tree.h structs.h symtable.h
	$(COMPILATOR) -c $< 

lexical_analyzer.o: lexical_analyzer.c lexical_analyzer.h structs.h 
	$(COMPILATOR) -c $< 

structs.o: structs.c structs.h symtable.h
	$(COMPILATOR) -c $< 

symtable.o: symtable.c symtable.h structs.h
	$(COMPILATOR) -c $< 

prec_parser.o: prec_parser.c prec_parser.h 
	$(COMPILATOR) -c $< 

d_tree.o: d_tree.c d_tree.h
	$(COMPILATOR) -c $<

sym_list.o: sym_list.c sym_list.h 
	$(COMPILATOR) -c $<
	
lex_list.o: lex_list.c lex_list.h 
	$(COMPILATOR) -c $<	

prec_table.o: prec_table.c prec_table.h  
	$(COMPILATOR) -c $<

create_tables.o: create_tables.c create_tables.h 
	$(COMPILATOR) -c $<

syntactic_analyzer.o: syntactic_analyzer.c syntactic_analyzer.h
	$(COMPILATOR) -c $<

sem_analysis.o: sem_analysis.c sem_analysis.h
	$(COMPILATOR) -c $<
	
code_gen.o: code_gen.c code_gen.h
	$(COMPILATOR) -c $<

stack.o: stack.c stack.h
	$(COMPILATOR) -c $<

clean:
	rm -f *.o
	rm translate


