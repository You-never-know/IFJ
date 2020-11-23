
all:
	cd ./lex_list/ && $(MAKE)
	cd ./structs/ && $(MAKE)
	cd ./sym_list/ && $(MAKE)
	cd ./symtable/ && $(MAKE)
	cd ./lexical_analyzer/ && $(MAKE)
	cd ./create_tables/ && $(MAKE)

clean:
	cd ./lexical_analyzer/ && $(MAKE)	clean
	cd ./create_tables/ && $(MAKE)	clean
	cd ./lex_list/ && $(MAKE)	clean
	cd ./structs/ && $(MAKE)	clean
	cd ./sym_list/ && $(MAKE)	clean
	cd ./symtable/ && $(MAKE)	clean

move:
	cd ./create_tables/ && $(MAKE)	move
	cd ./lexical_analyzer/ && $(MAKE)	move

run_test:
	@echo [MESSAGE] RUN LEX_TEST
	@echo ----------------------------------------------------------------------------------
	cd ./test_files/ && valgrind ./lex_test
	@echo ----------------------------------------------------------------------------------
	@echo [MESSAGE] RUN TEST_TABLE
	@echo ----------------------------------------------------------------------------------
	cd ./test_files/ && valgrind ./test_table
	@echo ----------------------------------------------------------------------------------