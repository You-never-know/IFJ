CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../structs/inc/ -I../symtable/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe
TARGET = lex_test

OBJ =lex_test.o\
		lexical_analyzer.o\

H = lexical_analyzer.h\
		

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET) ./../structs/obj/structs.o

$(OBJ_PATH)lexical_analyzer.o: $(SRC_PATH)lexical_analyzer.c $(HEADERS)
	@echo [MESSAGE] CREATING LEXICAL_ANALYZER.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(OBJ_PATH)lex_test.o: ./tests/lex_test.c
	@echo [MESSAGE] CREATING LEX_TEST.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(TARGET): $(OBJ_FILES) ./../structs/obj/structs.o
	@echo [MESSAGE] CREATING EXE LEX_TEST
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) $^
	@echo ----------------------------------------------------------------------------------

clean:
	@echo [MESSAGE] DELETING $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
	rm -f $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
move:
	@echo [MESSAGE] MOVING LEX_TEST
	@echo ----------------------------------------------------------------------------------
	mv $(TARGET) ./../test_files/
	@echo ----------------------------------------------------------------------------------






