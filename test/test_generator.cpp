#include <iostream>
#include <spy/generator/ast.hpp>
#include <spy/generator/generator.hpp>
#include <spy/generator/token.hpp>

using namespace std;

int main(int argc, char **args) {

  if (argc < 2) {
    cout << "Introduce the path of the code" << endl;
    exit(-1);
  }

  TOKEN *tokens = tokenize(read_file(args[1]));
  cout << "Tokenizing: Finished" << endl;
  print_tokens(tokens);
  AST *ast = parse(&tokens);
  cout << "AST: Finished" << endl;
  print_ast(ast, 0);
  string cpp_code = code_from_ast(ast);
  cout << cpp_code << endl;
  return 0;
}
