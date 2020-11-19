CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../symtable/inc/ -I../structs/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = symtable.o


H =	symtable.h

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)symtable.o: ./../symtable/src/symtable.c ./../symtable/inc/symtable.h ./../structs/inc/structs.h
	$(COMPILATOR) -c $<

clean:
	rm -f $(OBJ_FILES)




