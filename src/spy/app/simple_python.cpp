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
  cout << "[Tokenizing] Finished" << endl;
  AST *ast = parse(&tokens);
  cout << "[Generating AST] Finished" << endl;
  Generator *generator = new Generator();
  string cpp_code = generator->code_from_ast(ast);
  cout << "[Generating C++ code] Finished" << endl;
  write_file(cpp_code, "_tmp.cpp");
  cout << "[Writing code] Finished" << endl;
  system("g++ -c -I./include _tmp.cpp");
  system("g++ _tmp.o lib/object.o lib/error.o lib/spy_standard.o");
  system("rm _tmp.cpp");
  system("rm _tmp.o");
  return 0;
}
