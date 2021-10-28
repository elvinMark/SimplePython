#include <spy/generator/generator.hpp>

string code_from_file(string path_to_file) {
  return code_from_string(read_file(path_to_file));
}

string code_from_string(string code) {
  TOKEN *tokens = tokenize(code);
  return code_from_tokens(tokens);
}

string code_from_tokens(TOKEN *tokens) {
  AST *_ast = parse(&tokens);
  return code_from_ast(_ast);
}

string code_from_ast(AST *_ast) {
  string _code = "";
  _code.append(generate_header());
  return _code;
}

string generate_header() { return "#include <spy/core/core.hpp>\n"; }
