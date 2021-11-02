#include <spy/interpreter/simple_python_interpreter.hpp>

void run_command_prompt() {
  string _line = "";
  TOKEN *_tokens;
  AST *_ast;

  while (_line != "exit") {
    cout << ">>> ";
    getline(cin, _line);
    cout << _line << endl;
    _tokens = tokenize("\nx = 3\n\n");
    print_tokens(_tokens);
    _ast = parse(&_tokens);
    print_ast(_ast, 0);
  }
}

int main() {
  run_command_prompt();
  return 0;
}
