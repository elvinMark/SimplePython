#include <spy/generator/ast.hpp>

char ast_type_map[53][30] = {"AST_MODULE",
                             "AST_STMT",
                             "AST_STMTS",
                             "AST_EXPR",
                             "AST_EXPRS",
                             "AST_IDENTIFIER",
                             "AST_CONSTANT",
                             "AST_ASSIGN",
                             "AST_CLASSDEF",
                             "AST_FUNCTIONDEF",
                             "AST_ARGUMENTS",
                             "AST_RETURN",
                             "AST_IF",
                             "AST_ELSE",
                             "AST_INITERATOR",
                             "AST_FOR",
                             "AST_WHILE",
                             "AST_IMPORT",
                             "AST_IMPORTFROM",
                             "AST_PASS",
                             "AST_BREAK",
                             "AST_CONTINUE",
                             "AST_LIST",
                             "AST_DICT",
                             "AST_CALL",
                             "AST_SUBSCRIPT",
                             "AST_SLICE",
                             "AST_BOOLOP",
                             "AST_BINOP",
                             "AST_UNARYOP",
                             "AST_CMPOP",
                             "AST_IFEXPR",
                             "AST_LAMBDA",
                             "AST_AND",
                             "AST_OR",
                             "AST_ADD"
                             "AST_SUB",
                             "AST_MULT",
                             "AST_DIV",
                             "AST_MOD",
                             "AST_POW",
                             "AST_LSHIFT",
                             "AST_RSHIFT",
                             "AST_BITOR",
                             "AST_BITAND",
                             "AST_BITXOR",
                             "AST_NOT",
                             "AST_EQ",
                             "AST_LT",
                             "AST_LTE",
                             "AST_GT",
                             "AST_GTE",
                             "AST_IN"};

int is_expression(int token_type) {
  // TODO
  return 1;
}

AST::AST() {
  this->ast_type = AST_MODULE;
  this->ast_token = NULL;
  this->child_nodes = NULL;
  this->child_num = 0;
}

AST::AST(int ast_type, TOKEN *ast_token) {
  this->ast_type = ast_type;
  this->ast_token = ast_token;
  this->child_nodes = (AST **)malloc(sizeof(AST *));
  this->child_num = 0;
}

void AST::add_child(AST *child_node) {
  this->child_nodes[this->child_num] = child_node;
  this->child_num += 1;
  this->child_nodes =
      (AST **)realloc(this->child_nodes, sizeof(AST *) * (this->child_num + 1));
}

void AST::print(int depth) {
  for (int i = 0; i < depth; i++)
    cout << " ";
  cout << ast_type_map[this->ast_type] << endl;

  for (int i = 0; i < this->child_num; i++)
    this->child_nodes[i]->print(depth + 4);
}
