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

test_random:
	rm third_parties/*;
	$(CC) -I$(INCLUDE_DIR) -c test/test_random.cpp -o third_parties/test_random.o
	$(CC) third_parties/*.o -o third_parties/test_random

test_core:
	rm third_parties/*;
	$(CC) -I$(INCLUDE_DIR) -c src/spy/core/object.cpp -o third_parties/object.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/error.cpp -o third_parties/error.o
	$(CC) -I$(INCLUDE_DIR) -c test/test_core.cpp -o third_parties/test_core.o
	$(CC) third_parties/*.o -o third_parties/test_core

test_generator:
	rm third_parties/*
	$(CC) -I$(INCLUDE_DIR) -c src/spy/generator/token.cpp -o third_parties/token.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/generator/ast.cpp -o third_parties/ast.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/generator/generator.cpp -o third_parties/generator.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/error.cpp -o third_parties/error.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/misc.cpp -o third_parties/misc.o
	$(CC) -I$(INCLUDE_DIR) -c test/test_generator.cpp -o third_parties/test_generator.o
	$(CC) third_parties/*.o -o third_parties/test_generator
