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

extern string ast_type_map[53];

typedef struct AST {
  int _type;
  TOKEN *_token;
  AST *_next;
  AST *_prev;
  AST *_first_child;
  AST *_last_child;
} AST;

AST *create_ast(int _type, TOKEN *_token);
AST *create_ast(int _type, TOKEN *_token, AST *_next, AST *_prev,
                AST *_first_child, AST *_last_child);
void push_ast(AST **_ast, AST *new_ast);
AST *pop_ast(AST **_ast);
void push_child(AST *_ast, AST *child);
AST *pop_child(AST *_ast);
void print_ast(AST *_ast, int depth);
int is_token(TOKEN *token, int type);
int is_next_token(TOKEN *token, int type);
void consume_token(TOKEN **token, int type);
int is_end_of_expr(TOKEN *token);
int is_constant(TOKEN *token);
int is_token_boolop(TOKEN *token);
int is_token_binop(TOKEN *token);
int is_token_cmpop(TOKEN *token);
int is_ast(AST *_ast, int _type);
int is_prev_ast(AST *_ast, int _type);
int is_last_child_expr(AST *_ast);

AST *parse(TOKEN **tokens);
AST *parse_stmt(TOKEN **tokens);
AST *parse_stmts(TOKEN **tokens);
AST *parse_expr(TOKEN **tokens);
AST *parse_identifier(TOKEN **tokens);
AST *parse_constant(TOKEN **tokens);
AST *parse_classdef(TOKEN **tokens);
AST *parse_functiondef(TOKEN **tokens);
AST *parse_arguments(TOKEN **tokens);
AST *parse_return(TOKEN **tokens);
AST *parse_if(TOKEN **tokens);
AST *parse_else(TOKEN **tokens);
AST *parse_initerator(TOKEN **tokens);
AST *parse_for(TOKEN **tokens);
AST *parse_while(TOKEN **tokens);
AST *parse_import(TOKEN **tokens);
AST *parse_importfrom(TOKEN **tokens);
AST *parse_pass(TOKEN **tokens);
AST *parse_break(TOKEN **tokens);
AST *parse_continue(TOKEN **tokens);
AST *parse_list(TOKEN **tokens);
AST *parse_dict(TOKEN **tokens);
AST *parse_call(TOKEN **tokens);
AST *parse_subscript(TOKEN **tokens);
AST *parse_slice(TOKEN **tokens);
AST *parse_lambda(TOKEN **tokens);

#endif
