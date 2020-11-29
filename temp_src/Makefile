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


all: prec_parser.o symtable.o create_tables.o lex_test test_table prec_table prec_test
#all: translate

translate: translate.o lexical_analyzer.o structs.o symtable.o prec_parser.o d_tree.o sym_list.o lex_list.o prec_table.o create_tables.o
	$(COMPILATOR) $^

translate.o: translate.c create_tables.h prec_parser.h d_tree.h structs.h symtable.h
	$(COMPILATOR) -c $< 

lex_test: lex_test.o lexical_analyzer.o structs.o 
	$(COMPILATOR) $^
lex_test.o: lex_test.c
	$(COMPILATOR) -c lex_test.c
lexical_analyzer.o: lexical_analyzer.c lexical_analyzer.h structs.h 
	$(COMPILATOR) -c lexical_analyzer.c
structs.o: structs.c structs.h symtable.h
	$(COMPILATOR) -c structs.c

test_table: test_table.o symtable.o create_tables.o lexical_analyzer.o sym_list.o structs.o
	$(COMPILATOR) $^

prec_test: prec_test.o d_tree.o prec_parser.o prec_table.o lex_list.o sym_list.o symtable.o lexical_analyzser.o structs.o
	$(COMPILATOR) $^ 

symtable.o: symtable.c symtable.h structs.h
	$(COMPILATOR) -c $< 

prec_parser.o: prec_parser.c prec_parser.h 
	$(COMPILATOR) -c $< 

prec_test.o: prec_test.c 
	$(COMPILATOR) -c $<

d_tree.o: d_tree.c d_tree.h
	$(COMPILATOR) -c $<

test_table.o: test_table.c symtable.h structs.h
	$(COMPILATOR) -c $<

sym_list.o: sym_list.c sym_list.h 
	$(COMPILATOR) -c $<
	
lex_list.o: lex_list.c lex_list.h 
	$(COMPILATOR) -c $<	

prec_table.o: prec_table.c prec_table.h  
	$(COMPILATOR) -c $<

create_tables.o: create_tables.c create_tables.h 
	$(COMPILATOR) -c $<

clean_go:
	rm -f *.go
clean:
	rm -f *.o
	rm test_table
	rm lex_test

run: all
	./translate
