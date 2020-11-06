/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
* 
* Author:
* Drengubiak Vladimír	xdreng01
* Daniel Marek          xmarek72
**/

CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra
COMPILATOR = gcc $(CFLAGS) -o $@


all: lex_test test_table

lex_test: lex_test.o lexical_analyzer.o structs.o
	$(COMPILATOR) lex_test.o lexical_analyzer.o structs.o
lex_test.o: lex_test.c
	$(COMPILATOR) -c lex_test.c
lexical_analyzer.o: lexical_analyzer.c lexical_analyzer.h structs.h
	$(COMPILATOR) -c lexical_analyzer.c
structs.o: structs.c structs.h
	$(COMPILATOR) -c structs.c

test_table: test_table.o symtable.o
	$(COMPILATOR) $^

symtable.o: symtable.c symtable.h structs.h
	$(COMPILATOR) -c $< 

test_table.o: test_table.c symtable.h
	$(COMPILATOR) -c $<

clean_go:
	rm -f *.go
clean:
	rm -f *.o
	rm test_table
	rm lex_test
run: all
	valgrind ./lex_test
