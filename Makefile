CC=g++
INCLUDE_DIR=./include

test_token:
	rm third_parties/*
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/misc.cpp -o third_parties/misc.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/error.cpp -o third_parties/error.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/generator/token.cpp -o third_parties/token.o
	$(CC) -I$(INCLUDE_DIR) -c test/test_token.cpp -o third_parties/test_token.o
	$(CC) third_parties/*.o -o third_parties/test_token

test_ast:
	rm third_parties/*
	$(CC) -I$(INCLUDE_DIR) -c src/spy/generator/token.cpp -o third_parties/token.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/generator/ast.cpp -o third_parties/ast.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/error.cpp -o third_parties/error.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/misc.cpp -o third_parties/misc.o
	$(CC) -I$(INCLUDE_DIR) -c test/test_ast.cpp -o third_parties/test_ast.o
	$(CC) third_parties/*.o -o third_parties/test_ast
