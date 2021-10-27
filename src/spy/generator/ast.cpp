#include <spy/generator/ast.hpp>

string ast_type_map[53] = {"AST_MODULE",
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

AST *create_ast(int _type, TOKEN *_token, AST *_next, AST *_prev,
                AST *_first_child, AST *_last_child) {
  AST *_ast = (AST *)malloc(sizeof(AST));
  _ast->_type = _type;
  _ast->_token = _token;
  _ast->_next = _next;
  _ast->_prev = _prev;
  _ast->_first_child = _first_child;
  _ast->_last_child = _last_child;
  return _ast;
}

AST *create_ast(int _type, TOKEN *_token) {
  return create_ast(_type, _token, NULL, NULL, NULL, NULL);
}

void push_ast(AST **_ast, AST *_new_ast) {
  if ((*_ast) == NULL)
    (*_ast) = _new_ast;
  else {
    _new_ast->_prev = (*_ast);
    (*_ast)->_next = _new_ast;
    (*_ast) = _new_ast;
  }
}

AST *pop_ast(AST **_ast) {
  AST *_tmp;
  if ((*_ast) == NULL)
    return NULL;
  _tmp = *_ast;
  (*_ast) = (*_ast)->_prev;
  if ((*_ast) != NULL)
    (*_ast)->_next = NULL;
  _tmp->_prev = NULL;
  return _tmp;
}

void push_child(AST *_ast, AST *child) {
  push_ast(&_ast->_last_child, child);
  if (_ast->_first_child == NULL)
    _ast->_first_child = _ast->_last_child;
}

AST *pop_child(AST *_ast) {
  AST *_tmp = pop_ast(&_ast->_last_child);
  if (_ast->_last_child == NULL)
    _ast->_first_child = NULL;
  return _tmp;
}

void print_ast(AST *_ast, int depth) {
  AST *_dummy;
  if (_ast == NULL)
    return;
  for (int i = 0; i < depth; i++)
    cout << " ";
  cout << ast_type_map[_ast->_type] << endl;
  for (_dummy = _ast->_first_child; _dummy != NULL; _dummy = _dummy->_next)
    print_ast(_dummy, depth + 2);
}

int is_token(TOKEN *token, int _type) {
  if (token == NULL)
    return 0;
  return token->_type == _type;
}

int is_end_of_expr(TOKEN *token) {
  return token->_type == _ENDL || token->_type == _COLON ||
         token->_type == _COMMA || token->_type == _CLOSE_CURLY_BRACKET ||
         token->_type == _CLOSE_ROUND_BRACKET ||
         token->_type == _CLOSE_SQUARE_BRACKET || token->_type == _ASSIGN ||
         token->_type == _END;
}

int is_constant(TOKEN *token) {
  return token->_type == _DATA_INTEGER || token->_type == _DATA_REAL ||
         token->_type == _DATA_STRING;
}

void consume_token(TOKEN **_token, int _type) {
  if ((*_token) == NULL) {
    exit(-1);
  }
  if ((*_token)->_type == _type)
    (*_token) = (*_token)->_next;
}

AST *parse(TOKEN **tokens) {
  AST *_ast = create_ast(AST_MODULE, NULL);
  consume_token(tokens, _START);
  push_child(_ast, parse_stmts(tokens));
  consume_token(tokens, _END);
  return _ast;
}

AST *parse_stmts(TOKEN **tokens) {
  AST *_ast = create_ast(AST_STMTS, NULL);
  while (!is_token(*tokens, _END)) {
    while (is_token(*tokens, _ENDL))
      consume_token(tokens, _ENDL);
    push_child(_ast, parse_stmt(tokens));
  }
  while (is_token(*tokens, _ENDL))
    consume_token(tokens, _ENDL);
  return _ast;
}

AST *parse_stmt(TOKEN **tokens) {
  AST *_ast = create_ast(AST_STMT, NULL);
  if (is_token(*tokens, _CLASS)) {
    push_child(_ast, parse_classdef(tokens));
  } else if (is_token(*tokens, _DEF)) {
    push_child(_ast, parse_functiondef(tokens));
  } else if (is_token(*tokens, _RETURN)) {
    push_child(_ast, parse_return(tokens));
  } else if (is_token(*tokens, _IF)) {
    push_child(_ast, parse_if(tokens));
  } else if (is_token(*tokens, _FOR)) {
    push_child(_ast, parse_for(tokens));
  } else if (is_token(*tokens, _WHILE)) {
    push_child(_ast, parse_while(tokens));
  } else if (is_token(*tokens, _IMPORT)) {
    push_child(_ast, parse_import(tokens));
  } else if (is_token(*tokens, _FROM)) {
    push_child(_ast, parse_importfrom(tokens));
  } else if (is_token(*tokens, _PASS)) {
    push_child(_ast, parse_pass(tokens));
  } else if (is_token(*tokens, _BREAK)) {
    push_child(_ast, parse_break(tokens));
  } else if (is_token(*tokens, _CONTINUE)) {
    push_child(_ast, parse_continue(tokens));
  } else if (is_token(*tokens, _ENDL)) {
    // FIXME
    consume_token(tokens, _ENDL);
  } else {
    push_child(_ast, parse_expr(tokens));
    if (is_token(*tokens, _ASSIGN)) {
      AST *_tmp = create_ast(AST_ASSIGN, NULL);
      consume_token(tokens, _ASSIGN);
      push_child(_tmp, pop_child(_ast));
      push_child(_tmp, parse_expr(tokens));
      push_child(_ast, _tmp);
    }
    cout << "sdfsaf" << endl;
  }
  return _ast;
}

AST *parse_expr(TOKEN **tokens) {
  AST *_ast = create_ast(AST_EXPR, NULL);
  while (!is_end_of_expr(*tokens)) {
    if (is_token(*tokens, _VAR)) {
      // TODO: Peek for call or subscript
      push_child(_ast, parse_identifier(tokens));
    } else if (is_constant(*tokens)) {
      push_child(_ast, parse_constant(tokens));
    } else if (is_token(*tokens, _OPEN_SQUARE_BRACKET)) {
      push_child(_ast, parse_list(tokens));
    } else if (is_token(*tokens, _OPEN_CURLY_BRACKET)) {
      push_child(_ast, parse_dict(tokens));
    } else if (is_token(*tokens, _LAMBDA)) {
      push_child(_ast, parse_lambda(tokens));
    }
  }
  if (is_token(*tokens, _ENDL))
    consume_token(tokens, _ENDL);
  return _ast;
}

AST *parse_identifier(TOKEN **tokens) {
  AST *_ast = create_ast(AST_IDENTIFIER, *tokens);
  consume_token(tokens, _VAR);
  return _ast;
}

AST *parse_constant(TOKEN **tokens) {
  AST *_ast = create_ast(AST_CONSTANT, *tokens);
  if ((*tokens)->_type == _DATA_INTEGER)
    consume_token(tokens, _DATA_INTEGER);
  else if ((*tokens)->_type == _DATA_REAL)
    consume_token(tokens, _DATA_REAL);
  else if ((*tokens)->_type == _DATA_STRING)
    consume_token(tokens, _DATA_STRING);
  else
    exit(-1);
  return _ast;
}

AST *parse_classdef(TOKEN **tokens) {
  // TODO
  AST *_ast = create_ast(AST_CLASSDEF, NULL);
  push_child(_ast, parse_identifier(tokens));
  return _ast;
}

AST *parse_functiondef(TOKEN **tokens) {
  AST *_ast = create_ast(AST_FUNCTIONDEF, NULL);
  consume_token(tokens, _DEF);
  push_child(_ast, parse_identifier(tokens));
  consume_token(tokens, _OPEN_ROUND_BRACKET);
  push_child(_ast, parse_arguments(tokens));
  consume_token(tokens, _CLOSE_ROUND_BRACKET);
  consume_token(tokens, _COLON);
  consume_token(tokens, _ENDL);
  consume_token(tokens, _START);
  push_child(_ast, parse_stmts(tokens));
  consume_token(tokens, _END);
  return _ast;
}

AST *parse_arguments(TOKEN **tokens) {
  AST *_ast = create_ast(AST_ARGUMENTS, NULL);
  while (!is_token(*tokens, _CLOSE_ROUND_BRACKET)) {
    push_child(_ast, parse_identifier(tokens));
    if (!is_token(*tokens, _CLOSE_ROUND_BRACKET))
      consume_token(tokens, _COMMA);
  }
  return _ast;
}

AST *parse_return(TOKEN **tokens) {
  AST *_ast = create_ast(AST_RETURN, NULL);
  consume_token(tokens, _RETURN);
  push_child(_ast, parse_expr(tokens));
  return _ast;
}

AST *parse_if(TOKEN **tokens) {
  AST *_ast = create_ast(AST_IF, NULL);
  consume_token(tokens, _IF);
  push_child(_ast, parse_expr(tokens));
  consume_token(tokens, _COLON);
  consume_token(tokens, _ENDL);
  consume_token(tokens, _START);
  push_child(_ast, parse_stmts(tokens));
  consume_token(tokens, _END);
  if (is_token(*tokens, _ELIF)) {
    // TODO
  } else if (is_token(*tokens, _ELSE)) {
    push_child(_ast, parse_else(tokens));
  }
  return _ast;
}

AST *parse_else(TOKEN **tokens) {
  AST *_ast = create_ast(AST_ELSE, NULL);
  consume_token(tokens, _ELSE);
  consume_token(tokens, _COLON);
  consume_token(tokens, _ENDL);
  consume_token(tokens, _START);
  push_child(_ast, parse_stmts(tokens));
  consume_token(tokens, _END);
  return _ast;
}

AST *parse_initerator(TOKEN **tokens) {
  AST *_ast = create_ast(AST_INITERATOR, NULL);
  push_child(_ast, parse_identifier(tokens));
  consume_token(tokens, _IN);
  push_child(_ast, parse_expr(tokens));
  return _ast;
}

AST *parse_for(TOKEN **tokens) {
  AST *_ast = create_ast(AST_FOR, NULL);
  consume_token(tokens, _FOR);
  push_child(_ast, parse_initerator(tokens));
  consume_token(tokens, _COLON);
  consume_token(tokens, _ENDL);
  consume_token(tokens, _START);
  push_child(_ast, parse_stmts(tokens));
  consume_token(tokens, _END);
  return _ast;
}

AST *parse_while(TOKEN **tokens) {
  AST *_ast = create_ast(AST_WHILE, NULL);
  consume_token(tokens, _WHILE);
  push_child(_ast, parse_expr(tokens));
  consume_token(tokens, _COLON);
  consume_token(tokens, _ENDL);
  consume_token(tokens, _START);
  push_child(_ast, parse_stmts(tokens));
  consume_token(tokens, _END);
  return _ast;
}

AST *parse_import(TOKEN **tokens) {
  AST *_ast = create_ast(AST_IMPORT, NULL);
  consume_token(tokens, _IMPORT);
  push_child(_ast, parse_identifier(tokens));
  return _ast;
}

AST *parse_importfrom(TOKEN **tokens) {
  AST *_ast = create_ast(AST_IMPORTFROM, NULL);
  consume_token(tokens, _FROM);
  push_child(_ast, parse_identifier(tokens));
  consume_token(tokens, _IMPORT);
  push_child(_ast, parse_identifier(tokens));
  return _ast;
}

AST *parse_pass(TOKEN **tokens) {
  AST *_ast = create_ast(AST_PASS, NULL);
  consume_token(tokens, _PASS);
  return _ast;
}

AST *parse_break(TOKEN **tokens) {
  AST *_ast = create_ast(AST_BREAK, NULL);
  consume_token(tokens, _BREAK);
  return _ast;
}
AST *parse_continue(TOKEN **tokens) {
  AST *_ast = create_ast(AST_CONTINUE, NULL);
  consume_token(tokens, _CONTINUE);
  return _ast;
}

AST *parse_list(TOKEN **tokens) {
  AST *_ast = create_ast(AST_LIST, NULL);
  AST *_elems = create_ast(AST_EXPRS, NULL);
  consume_token(tokens, _OPEN_SQUARE_BRACKET);
  while (!is_token(*tokens, _CLOSE_SQUARE_BRACKET)) {
    push_child(_elems, parse_expr(tokens));
    if (!is_token(*tokens, _CLOSE_SQUARE_BRACKET))
      consume_token(tokens, _COMMA);
  }
  consume_token(tokens, _CLOSE_SQUARE_BRACKET);
  push_child(_ast, _elems);
  return _ast;
}

AST *parse_dict(TOKEN **tokens) {
  AST *_ast = create_ast(AST_DICT, NULL);
  AST *_keys = create_ast(AST_EXPRS, NULL);
  AST *_values = create_ast(AST_EXPRS, NULL);
  consume_token(tokens, _OPEN_CURLY_BRACKET);
  while (!is_token(*tokens, _CLOSE_CURLY_BRACKET)) {
    push_child(_keys, parse_expr(tokens));
    consume_token(tokens, _COLON);
    push_child(_values, parse_expr(tokens));
    if (!is_token(*tokens, _CLOSE_CURLY_BRACKET)) {
      consume_token(tokens, _COMMA);
    }
  }
  consume_token(tokens, _CLOSE_CURLY_BRACKET);
  push_child(_ast, _keys);
  push_child(_ast, _values);
  return _ast;
}

AST *parse_call(TOKEN **tokens) {
  AST *_ast = create_ast(AST_CALL, NULL);
  AST *_args = create_ast(AST_EXPRS, NULL);
  push_child(_ast, parse_identifier(tokens));
  consume_token(tokens, _OPEN_ROUND_BRACKET);
  while (!is_token(*tokens, _CLOSE_ROUND_BRACKET)) {
    push_child(_args, parse_expr(tokens));
    if (!is_token(*tokens, _CLOSE_ROUND_BRACKET)) {
      consume_token(tokens, _COMMA);
    }
  }
  consume_token(tokens, _CLOSE_ROUND_BRACKET);
  push_child(_ast, _args);
  return _ast;
}

AST *parse_subscript(TOKEN **tokens) {
  AST *_ast = create_ast(AST_SUBSCRIPT, NULL);
  push_child(_ast, parse_identifier(tokens));
  push_child(_ast, parse_slice(tokens));
  return _ast;
}

AST *parse_slice(TOKEN **tokens) {
  AST *_ast = create_ast(AST_SLICE, NULL);
  push_child(_ast, parse_expr(tokens));
  if (is_token(*tokens, _COLON)) {
    consume_token(tokens, _COLON);
    push_child(_ast, parse_expr(tokens));
  }
  if (is_token(*tokens, _COLON)) {
    consume_token(tokens, _COLON);
    push_child(_ast, parse_expr(tokens));
  }
  return _ast;
}

AST *parse_lambda(TOKEN **tokens) {
  AST *_ast = create_ast(AST_LAMBDA, NULL);
  consume_token(tokens, _LAMBDA);
  // lambda (x,y) : x+y
  consume_token(tokens, _OPEN_ROUND_BRACKET);
  push_child(_ast, parse_arguments(tokens));
  consume_token(tokens, _CLOSE_ROUND_BRACKET);
  consume_token(tokens, _COLON);
  push_child(_ast, parse_expr(tokens));
  return _ast;
}

// AST *parse_boolop(TOKEN **tokens);
// AST *parse_binop(TOKEN **tokens);
// AST *parse_unaryop(TOKEN **tokens);
// AST *parse_cmpop(TOKEN **tokens);
// AST *parse_ifexpr(TOKEN **tokens);
// AST *parse_and(TOKEN **tokens);
// AST *parse_or(TOKEN **tokens);
// AST *parse_add(TOKEN **tokens);
// AST *parse_sub(TOKEN **tokens);
// AST *parse_mult(TOKEN **tokens);
// AST *parse_div(TOKEN **tokens);
// AST *parse_mod(TOKEN **tokens);
// AST *parse_pow(TOKEN **tokens);
// AST *parse_lshift(TOKEN **tokens);
// AST *parse_rshift(TOKEN **tokens);
// AST *parse_bitor(TOKEN **tokens);
// AST *parse_bitand(TOKEN **tokens);
// AST *parse_bitxor(TOKEN **tokens);
// AST *parse_not(TOKEN **tokens);
// AST *parse_eq(TOKEN **tokens);
// AST *parse_lt(TOKEN **tokens);
// AST *parse_lte(TOKEN **tokens);
// AST *parse_gt(TOKEN **tokens);
// AST *parse_gte(TOKEN **tokens);
// AST *parse_in(TOKEN **tokens);
