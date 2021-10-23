#ifndef SPY_PARSER
#define SPY_PARSER

#include <cstring>
#include <fstream>
#include <iostream>
#include <spy/generator/ast.hpp>
#include <spy/generator/token.hpp>
#include <spy/utils/constants.hpp>
#include <spy/utils/error.hpp>
#include <string>

using namespace std;

// This is a simple parser for our simple python programming language

class Parser {
public:
  string raw_code;
  TOKEN *token;
  TOKEN *curr_token;

public:
  Parser(string raw_code);
  void generate_tokens();
  AST *generate_ast();
  AST *parse();
  AST *parse_stmt();
  AST *parse_expr();
  AST *parse_functiondef();
  AST *parse_classdef();
  AST *parse_return();
  AST *parse_assign();
  AST *parse_augassign();
  AST *parse_for();
  AST *parse_while();
  AST *parse_if();
  AST *parse_with();
  AST *parse_raise();
  AST *parse_try();
  AST *parse_assert();
  AST *parse_import();
  AST *parse_importfrom();
  AST *parse_global();
  AST *parse_pass();
  AST *parse_break();
  AST *parse_continue();
  AST *parse_boolop();
  AST *parse_namedexpr();
  AST *parse_binop();
  AST *parse_unaryop();
  AST *parse_lambda();
  AST *parse_ifexpr();
  AST *parse_dict();
  AST *parse_set();
  AST *parse_listcomp();
  AST *parse_setcomp();
  AST *parse_dictcomp();
  AST *parse_generatorexpr();
  AST *parse_compare();
  AST *parse_call();
  AST *parse_joinedstr();
  AST *parse_constant();

  static Parser *from_code_path(string code_path);
};

#endif
