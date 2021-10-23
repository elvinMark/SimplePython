#include <spy/generator/ast.hpp>

char ast_type_map[80][30] = {"AST_MODULE",
                             "AST_STMT",
                             "AST_FUNCTIONDEF",
                             "AST_CLASSDEF",
                             "AST_RETURN",
                             "AST_DELETE",
                             "AST_ASSIGN",
                             "AST_AUGASSIGN",
                             "AST_FOR",
                             "AST_WHILE",
                             "AST_IF",
                             "AST_WITH",
                             "AST_TRY",
                             "AST_RAISE",
                             "AST_IMPORT",
                             "AST_IMPORTFROM",
                             "AST_GLOBAL",
                             "AST_PASS",
                             "AST_BREAK",
                             "AST_CONTINUE",
                             "AST_EXPR",
                             "AST_BOOLOP",
                             "AST_BINOP",
                             "AST_UNARYOP",
                             "AST_LAMBDA",
                             "AST_IFEXPR",
                             "AST_DICT",
                             "AST_SET",
                             "AST_LISTCOMP",
                             "AST_SETCOMP",
                             "AST_DICTCOMP",
                             "AST_GENERATOREXP",
                             "AST_COMPARE",
                             "AST_CALL",
                             "AST_JOINEDSTR",
                             "AST_CONSTANT",
                             "AST_ATTRIBUTE",
                             "AST_SUBSCRIPT",
                             "AST_STARRED",
                             "AST_NAME",
                             "AST_LIST",
                             "AST_TUPLE",
                             "AST_SLICE",
                             "AST_EXPR_CONTEXT",
                             "AST_OPERATOR",
                             "AST_AND",
                             "AST_OR",
                             "AST_ADD",
                             "AST_SUB",
                             "AST_MULT",
                             "AST_DIV",
                             "AST_MOD",
                             "AST_POW",
                             "AST_LSHIFT",
                             "AST_RSHIFT",
                             "AST_BITOR",
                             "AST_BITXOR",
                             "AST_BITAND",
                             "AST_FLOORDIV",
                             "AST_MATMUL",
                             "AST_INVERT",
                             "AST_NOT",
                             "AST_UADD",
                             "AST_USUB",
                             "AST_CMPOP",
                             "AST_EQ",
                             "AST_NOTEQ",
                             "AST_LT",
                             "AST_LTE",
                             "AST_GT",
                             "AST_GTE",
                             "AST_IS",
                             "AST_ISNOT",
                             "AST_IN",
                             "AST_NOTIN",
                             "AST_COMPREHENSION",
                             "AST_EXCEPTHANDLER",
                             "AST_ARGUMENTS",
                             "AST_ARG",
                             "AST_KEYWORD",
                             "AST_ALIAS",
                             "AST_WITHITEM"};

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
