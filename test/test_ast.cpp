#include <iostream>
#include <spy/generator/ast.hpp>
#include <spy/generator/parser.hpp>
#include <spy/generator/token.hpp>

using namespace std;

int main(int argc, char **args) {

  if (argc < 2) {
    cout << "Introduce the path of the code" << endl;
    exit(-1);
  }

  Parser *parser = Parser::from_code_path(args[1]);
  TOKEN *token;
  AST *ast;
  parser->generate_tokens();

  for (token = parser->token; token != NULL; token = token->_next_token) {
    printf("token type: %s\n", token_type_map[token->_type]);
    if (token->_type == _DATA_INTEGER)
      printf("data: %d\n", token->_data._integer);
    else if (token->_type == _DATA_REAL)
      printf("data: %lf\n", token->_data._real);
    else if (token->_type == _DATA_STRING)
      printf("data: %s\n", token->_data._string);
  }

  ast = parser->parse();
  ast->print(0);
  return 0;
}
