#ifndef SPY_GENERATOR
#define SPY_GENERATOR

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <spy/generator/ast.hpp>
#include <spy/generator/token.hpp>
#include <spy/utils/constants.hpp>
#include <spy/utils/error.hpp>
#include <spy/utils/misc.hpp>
#include <string>
#include <vector>

using namespace std;

string get_string_from_token(TOKEN *_token);

class Generator {
public:
  map<string, int> variables;

public:
  Generator();

  string code_from_ast(AST *_ast);
  string code_from_tokens(TOKEN *tokens);
  string code_from_string(string code);
  string code_from_file(string path_to_file);

  string generate_module(AST *_ast);
  string generate_stmts(AST *_ast);
  string generate_stmt(AST *_ast);
  string generate_expr(AST *_ast);
  string generate_assign(AST *_ast);
  string generate_constant(AST *_ast);
  string generate_identifier(AST *_ast);
  string generate_binop(AST *_ast);
  string generate_boolop(AST *_ast);
  string generate_cmpop(AST *_ast);
};

#endif
