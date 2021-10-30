#include <spy/generator/generator.hpp>

string get_string_from_token(TOKEN *_token) {
  if (!is_token(_token, _DATA_STRING) && !is_token(_token, _VAR)) {
    assert_error(ERR_WRONG_TOKEN);
    return "";
  }
  return string(_token->_data->_string);
}

Generator::Generator() {}

string Generator::code_from_file(string path_to_file) {
  return this->code_from_string(read_file(path_to_file));
}

string Generator::code_from_string(string code) {
  TOKEN *tokens = tokenize(code);
  return this->code_from_tokens(tokens);
}

string Generator::code_from_tokens(TOKEN *tokens) {
  AST *_ast = parse(&tokens);
  return this->code_from_ast(_ast);
}

string Generator::code_from_ast(AST *_ast) {
  this->variables = map<string, int>();
  return this->generate_module(_ast);
}

string Generator::generate_module(AST *_ast) {
  if (!is_ast(_ast, AST_MODULE)) {
    assert_error(ERR_WRONG_AST);
  }
  return this->generate_stmts(_ast->_first_child);
}

string Generator::generate_stmts(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_STMTS)) {
    assert_error(ERR_WRONG_AST);
  }
  for (AST *_dummy = _ast->_first_child; _dummy != NULL; _dummy = _dummy->_next)
    _code.append(this->generate_stmt(_dummy));
  return _code;
}

string Generator::generate_stmt(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_STMT)) {
    assert_error(ERR_WRONG_AST);
  }
  if (is_ast(_ast->_first_child, AST_ASSIGN))
    _code += this->generate_assign(_ast->_first_child);
  return _code;
}

string Generator::generate_assign(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_ASSIGN)) {
    assert_error(ERR_WRONG_AST);
  }
  AST *_expr1 = _ast->_first_child;
  AST *_expr2 = _expr1 == NULL ? NULL : _expr1->_next;

  if (is_ast(_expr1->_first_child, AST_IDENTIFIER)) {
    _code += this->generate_expr(_expr1);
    _code += " = ";
    _code += this->generate_expr(_expr2);
    _code += ";";
  } else
    assert_error(ERR_NOT_IMPLEMENTED);
  return _code;
}

string Generator::generate_expr(AST *_ast) {
  string _code = "";
  if (!is_expr(_ast)) {
    assert_error(ERR_WRONG_AST);
  }
  if (is_ast(_ast, AST_EXPR)) {
    _code = this->generate_expr(_ast->_first_child);
  } else if (is_ast(_ast, AST_CONSTANT))
    _code += this->generate_constant(_ast);
  else if (is_ast(_ast, AST_IDENTIFIER))
    _code += this->generate_identifier(_ast);
  else if (is_ast(_ast, AST_BINOP))
    _code += this->generate_binop(_ast);
  else
    assert_error(ERR_NOT_IMPLEMENTED);

  return _code;
}

string Generator::generate_identifier(AST *_ast) {
  if (!is_ast(_ast, AST_IDENTIFIER)) {
    assert_error(ERR_WRONG_AST);
  }
  return get_string_from_token(_ast->_token);
}

string Generator::generate_constant(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_CONSTANT)) {
    assert_error(ERR_WRONG_AST);
  }
  if (is_token(_ast->_token, _DATA_INTEGER)) {
    _code += "create_integer_object(" +
             to_string(_ast->_token->_data->_integer) + ")";
  } else if (is_token(_ast->_token, _DATA_REAL)) {
    _code +=
        "create_real_object(" + to_string(_ast->_token->_data->_real) + ")";
  } else if (is_token(_ast->_token, _DATA_STRING)) {
    _code += "create_string_object(\"" + string(_ast->_token->_data->_string) +
             "\")";
  } else {
    assert_error(ERR_WRONG_TOKEN);
  }
  return _code;
}

string Generator::generate_binop(AST *_ast) {
  if (!is_ast(_ast, AST_BINOP)) {
    assert_error(ERR_WRONG_AST);
  }
  AST *_expr = _ast->_first_child;
  AST *_expr1 = _expr->_first_child;
  AST *_expr2 = _expr1 == NULL ? NULL : _expr1->_next;
  string _code = "";

  _code = "_object(" + this->generate_expr(_expr1) + "," +
          this->generate_expr(_expr2) + ")";
  if (is_ast(_expr, AST_ADD)) {
    _code = "add" + _code;
  } else if (is_ast(_expr, AST_SUB)) {
    _code = "sub" + _code;
  } else if (is_ast(_expr, AST_MULT)) {
    _code = "mult" + _code;
  } else if (is_ast(_expr, AST_DIV)) {
    _code = "div" + _code;
  } else if (is_ast(_expr, AST_POW)) {
    _code = "pow" + _code;
  } else if (is_ast(_expr, AST_BITAND)) {
    _code = "bitand" + _code;
  } else if (is_ast(_expr, AST_BITOR)) {
    _code = "bitor" + _code;
  } else if (is_ast(_expr, AST_BITXOR)) {
    _code = "bitxor" + _code;
  } else if (is_ast(_expr, AST_MOD)) {
    _code = "mod" + _code;
  } else if (is_ast(_expr, AST_LSHIFT)) {
    _code = "lshift" + _code;
  } else if (is_ast(_expr, AST_RSHIFT)) {
    _code = "rshift" + _code;
  } else
    assert_error(ERR_WRONG_TOKEN);

  return _code;
}

string Generator::generate_boolop(AST *_ast) {
  if (!is_ast(_ast, AST_BOOLOP)) {
    assert_error(ERR_WRONG_AST);
  }
  AST *_expr = _ast->_first_child;
  AST *_expr1 = _expr->_first_child;
  AST *_expr2 = _expr1 == NULL ? NULL : _expr1->_next;
  string _code = "";

  _code = "_object(" + this->generate_expr(_expr1) + "," +
          this->generate_expr(_expr2) + ")";
  if (is_ast(_expr, AST_ADD)) {
    _code = "add" + _code;
  } else if (is_ast(_expr, AST_SUB)) {
    _code = "sub" + _code;
  } else if (is_ast(_expr, AST_MULT)) {
    _code = "mult" + _code;
  } else if (is_ast(_expr, AST_DIV)) {
    _code = "div" + _code;
  } else if (is_ast(_expr, AST_POW)) {
    _code = "pow" + _code;
  } else if (is_ast(_expr, AST_AND)) {
    _code = "pow" + _code;
  } else
    assert_error(ERR_WRONG_TOKEN);

  return _code;
}

string Generator::generate_cmpop(AST *_ast) {
  if (!is_ast(_ast, AST_CMPOP)) {
    assert_error(ERR_WRONG_AST);
  }
  AST *_expr = _ast->_first_child;
  AST *_expr1 = _expr->_first_child;
  AST *_expr2 = _expr1 == NULL ? NULL : _expr1->_next;
  string _code = "";

  _code = "_object(" + this->generate_expr(_expr1) + "," +
          this->generate_expr(_expr2) + ")";
  if (is_ast(_expr, AST_ADD)) {
    _code = "add" + _code;
  } else if (is_ast(_expr, AST_SUB)) {
    _code = "sub" + _code;
  } else if (is_ast(_expr, AST_MULT)) {
    _code = "mult" + _code;
  } else if (is_ast(_expr, AST_DIV)) {
    _code = "div" + _code;
  } else if (is_ast(_expr, AST_POW)) {
    _code = "pow" + _code;
  } else if (is_ast(_expr, AST_AND)) {
    _code = "pow" + _code;
  } else
    assert_error(ERR_WRONG_TOKEN);

  return _code;
}
