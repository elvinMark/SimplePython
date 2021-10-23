#include <iostream>
#include <spy/generator/ast.hpp>
#include <spy/generator/parser.hpp>

int main(int argc, char **args) {
  if (argc < 2) {
    cout << "Introduce the path of the code" << endl;
    exit(-1);
  }

  AST *ast;
  Parser *parser = Parser::from_code_path(args[1]);
  parser = Parser::from_code_path(args[1]);

  return 0;
}
