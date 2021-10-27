#include <iostream>
#include <spy/generator/token.hpp>
#include <spy/utils/misc.hpp>

using namespace std;

int main(int argc, char **args) {

  if (argc < 2) {
    cout << "Introduce the path of the code" << endl;
    exit(-1);
  }

  TOKEN *tokens;
  tokens = tokenize(read_file(args[1]));
  cout << "Finish Tokenizing" << endl;
  print_tokens(tokens);
  return 0;
}
