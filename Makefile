CC=g++
INCLUDE_DIR=./include

test_parser:
	rm third_parties/*
	$(CC) -I$(INCLUDE_DIR) -c src/spy/generator/parser.cpp -o third_parties/parser.o
	$(CC) -I$(INCLUDE_DIR) -c src/spy/utils/error.cpp -o third_parties/error.o
	$(CC) -I$(INCLUDE_DIR) -c test/test_parser.cpp -o third_parties/test_parser.o
	$(CC) third_parties/*.o -o third_parties/a.out
