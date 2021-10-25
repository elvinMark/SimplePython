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
  AST *buffer;

public:
  Parser(string raw_code);
  void generate_tokens();
  void consume_token(int _token);
  int peek_token(int ahead);
  int is_token(int _token);
  AST *generate_ast();
  AST *parse();
  AST *parse_stmt();
  AST *parse_stmts();
  AST *parse_expr();
  AST *parse_identifier();
  AST *parse_constant();
  AST *parse_assign();
  AST *parse_classdef();
  AST *parse_functiondef();
  AST *parse_arguments();
  AST *parse_return();
  AST *parse_if();
  AST *parse_else();
  AST *parse_initerator();
  AST *parse_for();
  AST *parse_while();
  AST *parse_import();
  AST *parse_importfrom();
  AST *parse_pass();
  AST *parse_break();
  AST *parse_continue();
  AST *parse_list();
  AST *parse_dict();
  AST *parse_call();
  AST *parse_subscript();
  AST *parse_slice();
  AST *parse_boolop();
  AST *parse_binop();
  AST *parse_unaryop();
  AST *parse_cmpop();
  AST *parse_ifexpr();
  AST *parse_lambda();
  AST *parse_and();
  AST *parse_or();
  AST *parse_add();
  AST *parse_sub();
  AST *parse_mult();
  AST *parse_div();
  AST *parse_mod();
  AST *parse_pow();
  AST *parse_lshift();
  AST *parse_rshift();
  AST *parse_bitor();
  AST *parse_bitand();
  AST *parse_bitxor();
  AST *parse_not();
  AST *parse_eq();
  AST *parse_lt();
  AST *parse_lte();
  AST *parse_gt();
  AST *parse_gte();
  AST *parse_in();

  static Parser *from_code_path(string code_path);
};

#endif
