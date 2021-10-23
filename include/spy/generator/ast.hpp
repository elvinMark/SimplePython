#ifndef SPY_AST
#define SPY_AST

#include <iostream>
#include <spy/generator/token.hpp>

enum AST_TYPE {
  AST_MODULE,
  AST_STMT,
  AST_FUNCTIONDEF,
  AST_CLASSDEF,
  AST_RETURN,
  AST_DELETE,
  AST_ASSIGN,
  AST_AUGASSIGN,
  AST_FOR,
  AST_WHILE,
  AST_IF,
  AST_WITH,
  AST_TRY,
  AST_RAISE,
  AST_IMPORT,
  AST_IMPORTFROM,
  AST_GLOBAL,
  AST_PASS,
  AST_BREAK,
  AST_CONTINUE,
  AST_EXPR,
  AST_BOOLOP,
  AST_BINOP,
  AST_UNARYOP,
  AST_LAMBDA,
  AST_IFEXPR,
  AST_DICT,
  AST_SET,
  AST_LISTCOMP,
  AST_SETCOMP,
  AST_DICTCOMP,
  AST_GENERATOREXP,
  AST_COMPARE,
  AST_CALL,
  AST_JOINEDSTR,
  AST_CONSTANT,
  AST_ATTRIBUTE,
  AST_SUBSCRIPT,
  AST_STARRED,
  AST_NAME,
  AST_LIST,
  AST_TUPLE,
  AST_SLICE,
  AST_EXPR_CONTEXT,
  AST_OPERATOR,
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
  AST_BITXOR,
  AST_BITAND,
  AST_FLOORDIV,
  AST_MATMUL,
  AST_INVERT,
  AST_NOT,
  AST_UADD,
  AST_USUB,
  AST_CMPOP,
  AST_EQ,
  AST_NOTEQ,
  AST_LT,
  AST_LTE,
  AST_GT,
  AST_GTE,
  AST_IS,
  AST_ISNOT,
  AST_IN,
  AST_NOTIN,
  AST_COMPREHENSION,
  AST_EXCEPTHANDLER,
  AST_ARGUMENTS,
  AST_ARG,
  AST_KEYWORD,
  AST_ALIAS,
  AST_WITHITEM,
};

extern char ast_type_map[80][30];

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
