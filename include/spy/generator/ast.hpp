#ifndef SPY_AST
#define SPY_AST

#include <iostream>
#include <spy/generator/token.hpp>

using namespace std;

enum AST_TYPE {
  AST_MODULE,
  AST_STMT,
  AST_STMTS,
  AST_EXPR,
  AST_EXPRS,
  AST_IDENTIFIER,
  AST_CONSTANT,
  AST_ASSIGN,
  AST_CLASSDEF,
  AST_FUNCTIONDEF,
  AST_ARGUMENTS,
  AST_RETURN,
  AST_IF,
  AST_ELSE,
  AST_INITERATOR,
  AST_FOR,
  AST_WHILE,
  AST_IMPORT,
  AST_IMPORTFROM,
  AST_PASS,
  AST_BREAK,
  AST_CONTINUE,
  AST_LIST,
  AST_DICT,
  AST_CALL,
  AST_SUBSCRIPT,
  AST_SLICE,
  AST_BOOLOP,
  AST_BINOP,
  AST_UNARYOP,
  AST_CMPOP,
  AST_IFEXPR,
  AST_LAMBDA,
  AST_AND,
  AST_OR,
  AST_ADD,
  AST_SUB,
  AST_MULT,
  AST_DIV,
  AST_MOD,
  AST_POW,
  AST_LSHIFT,
  AST_RSHIFT,
  AST_BITOR,
  AST_BITAND,
  AST_BITXOR,
  AST_NOT,
  AST_EQ,
  AST_LT,
  AST_LTE,
  AST_GT,
  AST_GTE,
  AST_IN,
};

extern char ast_type_map[53][30];

int is_expression(int token_type);

class AST {
public:
  int ast_type;
  TOKEN *ast_token;
  AST **child_nodes;
  int child_num;

public:
  AST();
  AST(int ast_type, TOKEN *ast_token);
  void add_child(AST *child_node);
  void print(int depth);
};

#endif
