#include <iostream>
#include <spy/generator/parser.hpp>
using namespace std;

int main(int argc, char **args) {

  char type_map[48][30] = {"_NONE",
                           "_DATA_INTEGER",
                           "_DATA_REAL",
                           "_DATA_STRING",
                           "_CLASS",
                           "_DEF",
                           "_IF",
                           "_ELSE",
                           "_ELIF",
                           "_WHILE",
                           "_FOR",
                           "_IN",
                           "_RETURN",
                           "_LAMBDA",
                           "_AND",
                           "_OR",
                           "_NOT",
                           "_VAR",
                           "_TRUE",
                           "_FALSE",
                           "_OPEN_CURLY_BRACKET",
                           "_CLOSE_CURLY_BRACKET",
                           "_OPEN_ROUND_BRACKET",
                           "_CLOSE_ROUND_BRACKET",
                           "_OPEN_SQUARE_BRACKET",
                           "_CLOSE_SQUARE_BRACKET",
                           "_COLON",
                           "_COMMA",
                           "_DOT",
                           "_QUOTE",
                           "_DOUBLE_QUOTE",
                           "_START",
                           "_END",
                           "_ASSIGN",
                           "_OP_ADD",
                           "_OP_DIF",
                           "_OP_MUL",
                           "_OP_DIV",
                           "_OP_GT",
                           "_OP_LT",
                           "_OP_EQUAL",
                           "_OP_AND",
                           "_OP_OR",
                           "_OP_XOR",
                           "_OP_MOD",
                           "_OP_NOT",
                           "_OP_SHIFT_LEFT",
                           "_OP_SHIFT_RIGHT"};

  if (argc < 2) {
    cout << "Introduce the path of the code" << endl;
    exit(-1);
  }

  Parser *parser = Parser::from_code_path(args[1]);
  TOKEN *token;

  parser->generate_tokens();

  for (token = parser->token; token != NULL; token = token->_next_token) {
    printf("type: %s\n", type_map[token->_type]);
    if (token->_type == _DATA_INTEGER)
      printf("number: %d\n", token->_data._integer);
    else if (token->_type == _DATA_REAL)
      printf("number: %lf\n", token->_data._real);
    else if (token->_type == _DATA_STRING)
      printf("number: %s\n", token->_data._string);
  }

  return 0;
}
