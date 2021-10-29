#include <spy/generator/generator.hpp>

Generator::Generator() {}

string Generator::code_from_file(string path_to_file) {
  return this->code_from_string(read_file(path_to_file));
}

string Generator::code_from_string(string code) {
  TOKEN *tokens = tokenize(code);
  return this->code_from_tokens(tokens);
}

string Generator::code_from_tokens(TOKEN *tokens) {
  AST *_ast = parse(&tokens);
  return this->code_from_ast(_ast);
}

string Generator::code_from_ast(AST *_ast) {
  this->variables = map<string, int>();
  return this->generate_module(_ast);
}

string Generator::generate_module(AST *_ast) {
  if (!is_ast(_ast, AST_MODULE)) {
    assert_error(ERR_WRONG_AST);
  }
  return this->generate_stmts(_ast->_first_child);
}

string Generator::generate_stmts(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_STMTS)) {
    assert_error(ERR_WRONG_AST);
  }
  for (AST *_dummy = _ast->_first_child; _dummy != NULL; _dummy = _dummy->_next)
    _code.append(this->generate_stmt(_dummy));
  return _code;
}

string Generator::generate_stmt(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_STMT)) {
    assert_error(ERR_WRONG_AST);
  }

  return _code;
}

string Generator::generate_expr(AST *_ast) {
  string _code = "";
  return _code;
}

string Generator::generate_assign(AST *_ast) {
  string _code = "";
  return _code;
}
