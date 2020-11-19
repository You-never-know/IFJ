CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../structs/inc/ -I../symtable/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = lex_list.o

H =	lex_list.h 

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)lex_list.o: $(SRC_PATH)lex_list.c $(HEADERS) ./../structs/inc/structs.h ./../symtable/inc/symtable.h
	$(COMPILATOR) -c $<	

clean:
	rm -f $(OBJ_FILES)




