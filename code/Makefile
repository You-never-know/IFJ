
all:
	cd ./lex_list/ && $(MAKE)
	cd ./structs/ && $(MAKE)
	cd ./sym_list/ && $(MAKE)
	cd ./symtable/ && $(MAKE)
	cd ./lexical_analyzer/ && $(MAKE)
	cd ./create_tables/ && $(MAKE)
	cd ./d_tree/ && $(MAKE)
	cd ./prec_parser/ && $(MAKE)
	cd ./prec_table/ && $(MAKE)

clean:
	cd ./lexical_analyzer/ && $(MAKE)	clean
	cd ./create_tables/ && $(MAKE)	clean
	cd ./lex_list/ && $(MAKE)	clean
	cd ./structs/ && $(MAKE)	clean
	cd ./sym_list/ && $(MAKE)	clean
	cd ./symtable/ && $(MAKE)	clean
	cd ./d_tree/ && $(MAKE) clean
	cd ./prec_parser/ && $(MAKE) clean
	cd ./prec_table/ && $(MAKE) clean

run_table:
	@echo [MESSAGE] RUN TEST_TABLE
	@echo ----------------------------------------------------------------------------------
	cd ./create_tables/ && valgrind ./test_table
	@echo ----------------------------------------------------------------------------------

run_lex:
	cd ./lexical_analyzer/ && $(MAKE)	move
	@echo [MESSAGE] RUN LEX_TEST
	@echo ----------------------------------------------------------------------------------
	cd ./test_files/ && valgrind ./lex_test
	@echo ----------------------------------------------------------------------------------

run_d_tree:
	@echo [MESSAGE] RUN d TEST_D_TREE
	@echo ----------------------------------------------------------------------------------
	cd ./d_tree/ && valgrind ./test_d_tree
	@echo ----------------------------------------------------------------------------------