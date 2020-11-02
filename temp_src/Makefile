CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra
COMPILATOR = gcc $(CFLAGS) -o $@


all: lex_test

lex_test: lex_test.o lexical_analyzer.o structs.o
	$(COMPILATOR) lex_test.o lexical_analyzer.o structs.o
lex_test.o: lex_test.c
	$(COMPILATOR) -c lex_test.c
lexical_analyzer.o: lexical_analyzer.c lexical_analyzer.h structs.h
	$(COMPILATOR) -c lexical_analyzer.c
structs.o: structs.c structs.h
	$(COMPILATOR) -c structs.c
clean_go:
	rm -f *.go
clean:
	rm -f *.o
run: all
	valgrind ./lex_test
