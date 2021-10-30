#include <spy/generator/ast.hpp>

string ast_type_map[53] = {
    "AST_MODULE",   "AST_STMT",        "AST_STMTS",      "AST_EXPR",
    "AST_EXPRS",    "AST_IDENTIFIER",  "AST_CONSTANT",   "AST_ASSIGN",
    "AST_CLASSDEF", "AST_FUNCTIONDEF", "AST_ARGUMENTS",  "AST_RETURN",
    "AST_IF",       "AST_ELSE",        "AST_INITERATOR", "AST_FOR",
    "AST_WHILE",    "AST_IMPORT",      "AST_IMPORTFROM", "AST_PASS",
    "AST_BREAK",    "AST_CONTINUE",    "AST_LIST",       "AST_DICT",
    "AST_CALL",     "AST_SUBSCRIPT",   "AST_SLICE",      "AST_BOOLOP",
    "AST_BINOP",    "AST_UNARYOP",     "AST_CMPOP",      "AST_IFEXPR",
    "AST_LAMBDA",   "AST_AND",         "AST_OR",         "AST_ADD",
    "AST_SUB",      "AST_MULT",        "AST_DIV",        "AST_MOD",
    "AST_POW",      "AST_LSHIFT",      "AST_RSHIFT",     "AST_BITOR",
    "AST_BITAND",   "AST_BITXOR",      "AST_NOT",        "AST_EQ",
    "AST_LT",       "AST_LTE",         "AST_GT",         "AST_GTE",
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
         token->_type == _END || token->_type == _ELSE;
}

int is_constant(TOKEN *token) {
  return token->_type == _DATA_INTEGER || token->_type == _DATA_REAL ||
         token->_type == _DATA_STRING || token->_type == _TRUE ||
         token->_type == _FALSE;
}

int is_next_token(TOKEN *token, int _type) {
  if (token == NULL)
    return 0;
  return is_token(token->_next, _type);
}

int is_token_boolop(TOKEN *token) {
  if (token == NULL)
    return 0;
  return token->_type == _AND || token->_type == _OR;
}

int is_token_binop(TOKEN *token) {
  if (token == NULL)
    return 0;
  return token->_type == _OP_ADD || token->_type == _OP_DIF ||
         token->_type == _OP_MUL || token->_type == _OP_DIV ||
         token->_type == _OP_POW || token->_type == _OP_MOD ||
         token->_type == _OP_SHIFT_LEFT || token->_type == _OP_SHIFT_RIGHT ||
         token->_type == _OP_OR || token->_type == _OP_AND ||
         token->_type == _OP_XOR;
}

int is_token_cmpop(TOKEN *token) {
  if (token == NULL || token->_next == NULL)
    return 0;
  return token->_type == _OP_EQUAL || token->_type == _OP_LT ||
         token->_type == _OP_LTE || token->_type == _OP_GT ||
         token->_type == _OP_GTE || token->_type == _IN;
}

int is_ast(AST *_ast, int _type) {
  if (_ast == NULL)
    return 0;
  return _ast->_type == _type;
}

int is_prev_ast(AST *_ast, int _type) {
  if (_ast == NULL || _ast->_prev == NULL)
    return 0;
  return _ast->_prev->_type == _type;
}

int is_expr(AST *_ast) {
  return is_ast(_ast, AST_EXPR) || is_ast(_ast, AST_IDENTIFIER) ||
         is_ast(_ast, AST_CONSTANT) || is_ast(_ast, AST_CALL) ||
         is_ast(_ast, AST_LIST) || is_ast(_ast, AST_DICT) ||
         is_ast(_ast, AST_SUBSCRIPT) || is_ast(_ast, AST_BINOP) ||
         is_ast(_ast, AST_BOOLOP) || is_ast(_ast, AST_CMPOP);
}

int is_last_child_expr(AST *_ast) { return is_expr(_ast->_last_child); }

void consume_token(TOKEN **_token, int _type) {
  if ((*_token) == NULL) {
    assert_error(ERR_NO_TOKENS);
    exit(-1);
  }
  if ((*_token)->_type == _type)
    (*_token) = (*_token)->_next;
  else {
    assert_error(ERR_WRONG_TOKEN);
    exit(-1);
  }
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
  }
  return _ast;
}

AST *parse_expr(TOKEN **tokens) {
  AST *_ast = create_ast(AST_EXPR, NULL);
  while (!is_end_of_expr(*tokens)) {
    if (is_token(*tokens, _VAR)) {
      push_child(_ast, parse_identifier(tokens));
    } else if (is_constant(*tokens)) {
      push_child(_ast, parse_constant(tokens));
    } else if (is_token(*tokens, _OPEN_SQUARE_BRACKET)) {
      if (is_ast(_ast->_last_child, AST_IDENTIFIER)) {
        // Subscript
        AST *_id = pop_child(_ast);
        AST *_arr = create_ast(AST_SUBSCRIPT, NULL);
        push_child(_arr, _id);
        consume_token(tokens, _OPEN_SQUARE_BRACKET);
        push_child(_arr, parse_slice(tokens));
        consume_token(tokens, _CLOSE_SQUARE_BRACKET);
        push_child(_ast, _arr);
      } else
        push_child(_ast, parse_list(tokens));
    } else if (is_token(*tokens, _OPEN_CURLY_BRACKET)) {
      push_child(_ast, parse_dict(tokens));
    } else if (is_token(*tokens, _OPEN_ROUND_BRACKET)) {
      if (is_ast(_ast->_last_child, AST_IDENTIFIER)) {
        // Call
        AST *_call = create_ast(AST_CALL, NULL);
        AST *_exprs = create_ast(AST_EXPRS, NULL);
        push_child(_call, pop_child(_ast));
        consume_token(tokens, _OPEN_ROUND_BRACKET);
        while (!is_token(*tokens, _CLOSE_ROUND_BRACKET)) {
          push_child(_exprs, parse_expr(tokens));
          if (!is_token(*tokens, _CLOSE_ROUND_BRACKET))
            consume_token(tokens, _COMMA);
        }
        consume_token(tokens, _CLOSE_ROUND_BRACKET);
        push_child(_call, _exprs);
        push_child(_ast, _call);
      }
    } else if (is_token(*tokens, _LAMBDA)) {
      push_child(_ast, parse_lambda(tokens));
    } else if (is_token(*tokens, _NOT)) {
      AST *_tmp = create_ast(AST_NOT, NULL);
      consume_token(tokens, _NOT);
      push_child(_tmp, parse_expr(tokens));
      push_child(_ast, _tmp);
    } else if (is_token(*tokens, _IF)) {
      // IFEXPR
      if (is_last_child_expr(_ast)) {
        AST *_ifexpr = create_ast(AST_IFEXPR, NULL);
        push_child(_ifexpr, pop_child(_ast));
        consume_token(tokens, _IF);
        push_child(_ifexpr, parse_expr(tokens));
        consume_token(tokens, _ELSE);
        push_child(_ifexpr, parse_expr(tokens));
        push_child(_ast, _ifexpr);
      } else {
        assert_error(ERR_WRONG_TOKEN);
        exit(-1);
      }
    } else if (is_token_binop(*tokens)) {
      // BINOP
      if (!is_last_child_expr(_ast)) {
        assert_error(ERR_WRONG_TOKEN);
        exit(-1);
      }
      AST *_ast_op = create_ast(AST_BINOP, NULL);
      if (is_token(*tokens, _OP_ADD)) {
        AST *_ast_add = create_ast(AST_ADD, NULL);
        consume_token(tokens, _OP_ADD);
        push_child(_ast_add, pop_child(_ast));
        push_child(_ast_add, parse_expr(tokens));
        push_child(_ast_op, _ast_add);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_DIF)) {
        AST *_ast_diff = create_ast(AST_SUB, NULL);
        consume_token(tokens, _OP_DIF);
        push_child(_ast_diff, pop_child(_ast));
        push_child(_ast_diff, parse_expr(tokens));
        push_child(_ast_op, _ast_diff);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_MUL)) {
        AST *_ast_mult = create_ast(AST_MULT, NULL);
        consume_token(tokens, _OP_MUL);
        push_child(_ast_mult, pop_child(_ast));
        push_child(_ast_mult, parse_expr(tokens));
        push_child(_ast_op, _ast_mult);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_DIV)) {
        AST *_ast_div = create_ast(AST_DIV, NULL);
        consume_token(tokens, _OP_DIV);
        push_child(_ast_div, pop_child(_ast));
        push_child(_ast_div, parse_expr(tokens));
        push_child(_ast_op, _ast_div);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_POW)) {
        AST *_ast_pow = create_ast(AST_POW, NULL);
        consume_token(tokens, _OP_POW);
        push_child(_ast_pow, pop_child(_ast));
        push_child(_ast_pow, parse_expr(tokens));
        push_child(_ast_op, _ast_pow);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_SHIFT_LEFT)) {
        AST *_ast_lshift = create_ast(AST_LSHIFT, NULL);
        consume_token(tokens, _OP_SHIFT_LEFT);
        push_child(_ast_lshift, pop_child(_ast));
        push_child(_ast_lshift, parse_expr(tokens));
        push_child(_ast_op, _ast_lshift);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_SHIFT_RIGHT)) {
        AST *_ast_rshift = create_ast(AST_RSHIFT, NULL);
        consume_token(tokens, _OP_SHIFT_RIGHT);
        push_child(_ast_rshift, pop_child(_ast));
        push_child(_ast_rshift, parse_expr(tokens));
        push_child(_ast_op, _ast_rshift);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_AND)) {
        AST *_ast_bitand = create_ast(AST_BITAND, NULL);
        consume_token(tokens, _OP_AND);
        push_child(_ast_bitand, pop_child(_ast));
        push_child(_ast_bitand, parse_expr(tokens));
        push_child(_ast_op, _ast_bitand);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_OR)) {
        AST *_ast_bitor = create_ast(AST_BITOR, NULL);
        consume_token(tokens, _OP_OR);
        push_child(_ast_bitor, pop_child(_ast));
        push_child(_ast_bitor, parse_expr(tokens));
        push_child(_ast_op, _ast_bitor);
        push_child(_ast, _ast_op);
      } else if (is_token(*tokens, _OP_XOR)) {
        AST *_ast_bitxor = create_ast(AST_BITXOR, NULL);
        consume_token(tokens, _OP_XOR);
        push_child(_ast_bitxor, pop_child(_ast));
        push_child(_ast_bitxor, parse_expr(tokens));
        push_child(_ast_op, _ast_bitxor);
        push_child(_ast, _ast_op);
      } else {
        assert_error(ERR_WRONG_TOKEN);
        exit(-1);
      }
    } else if (is_token_boolop(*tokens)) {
      // BOOLOP
      if (!is_last_child_expr(_ast)) {
        assert_error(ERR_WRONG_TOKEN);
        exit(-1);
      }
      AST *_ast_boolop = create_ast(AST_BOOLOP, NULL);
      if (is_token(*tokens, _AND)) {
        AST *_ast_and = create_ast(AST_AND, NULL);
        consume_token(tokens, _AND);
        push_child(_ast_and, pop_child(_ast));
        push_child(_ast_and, parse_expr(tokens));
        push_child(_ast_boolop, _ast_and);
        push_child(_ast, _ast_boolop);
      } else if (is_token(*tokens, _OR)) {
        AST *_ast_or = create_ast(AST_OR, NULL);
        consume_token(tokens, _OR);
        push_child(_ast_or, pop_child(_ast));
        push_child(_ast_or, parse_expr(tokens));
        push_child(_ast_boolop, _ast_or);
        push_child(_ast, _ast_boolop);
      } else {
        assert_error(ERR_WRONG_TOKEN);
        exit(-1);
      }
    } else if (is_token_cmpop(*tokens)) {
      // CMPOP
      if (!is_last_child_expr(_ast)) {
        assert_error(ERR_WRONG_TOKEN);
        exit(-1);
      }
      AST *_ast_cmpop = create_ast(AST_CMPOP, NULL);
      if (is_token(*tokens, _OP_EQUAL)) {
        AST *_ast_equal = create_ast(AST_EQ, NULL);
        consume_token(tokens, _OP_EQUAL);
        push_child(_ast_equal, pop_child(_ast));
        push_child(_ast_equal, parse_expr(tokens));
        push_child(_ast_cmpop, _ast_equal);
        push_child(_ast, _ast_cmpop);
      } else if (is_token(*tokens, _OP_LT)) {
        AST *_ast_lt = create_ast(AST_LT, NULL);
        consume_token(tokens, _OP_LT);
        push_child(_ast_lt, pop_child(_ast));
        push_child(_ast_lt, parse_expr(tokens));
        push_child(_ast_cmpop, _ast_lt);
        push_child(_ast, _ast_cmpop);
      } else if (is_token(*tokens, _OP_LTE)) {
        AST *_ast_lte = create_ast(AST_LTE, NULL);
        consume_token(tokens, _OP_LTE);
        push_child(_ast_lte, pop_child(_ast));
        push_child(_ast_lte, parse_expr(tokens));
        push_child(_ast_cmpop, _ast_lte);
        push_child(_ast, _ast_cmpop);
      } else if (is_token(*tokens, _OP_GT)) {
        AST *_ast_gt = create_ast(AST_GT, NULL);
        consume_token(tokens, _OP_GT);
        push_child(_ast_gt, pop_child(_ast));
        push_child(_ast_gt, parse_expr(tokens));
        push_child(_ast_cmpop, _ast_gt);
        push_child(_ast, _ast_cmpop);
      } else if (is_token(*tokens, _OP_GTE)) {
        AST *_ast_gte = create_ast(AST_GTE, NULL);
        consume_token(tokens, _OP_GTE);
        push_child(_ast_gte, pop_child(_ast));
        push_child(_ast_gte, parse_expr(tokens));
        push_child(_ast_cmpop, _ast_gte);
        push_child(_ast, _ast_cmpop);
      } else if (is_token(*tokens, _IN)) {
        AST *_ast_in = create_ast(AST_IN, NULL);
        consume_token(tokens, _IN);
        push_child(_ast_in, pop_child(_ast));
        push_child(_ast_in, parse_expr(tokens));
        push_child(_ast_cmpop, _ast_in);
        push_child(_ast, _ast_cmpop);
      } else {
        assert_error(ERR_WRONG_TOKEN);
        exit(-1);
      }
    }
  }
  // if (is_token(*tokens, _ENDL))
  //   consume_token(tokens, _ENDL);
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
  else if ((*tokens)->_type == _TRUE)
    consume_token(tokens, _TRUE);
  else if ((*tokens)->_type == _FALSE)
    consume_token(tokens, _FALSE);
  else {
    assert_error(ERR_WRONG_TOKEN);
    exit(-1);
  }
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
