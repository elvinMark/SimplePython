#ifndef SPY_INTERPRETER
#define SPY_INTERPRETER

#include <iostream>
#include <map>
#include <spy/core/object.hpp>
#include <spy/generator/ast.hpp>
#include <spy/generator/token.hpp>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class SpyInterpreter {
public:
  map<string, _object *> _variables;
  map<string, AST *> _functions;
  map<string, _object *> _local_variables;
  stack<_object *> _args;

public:
  SpyInterpreter();
  void interprete(AST *_ast);
  void interprete_stmt(AST *_ast);
  void interprete_expr(AST *_ast);
};

#endif
