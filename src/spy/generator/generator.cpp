#include <spy/generator/generator.hpp>

string get_string_from_token(TOKEN *_token) {
  if (!is_token(_token, _DATA_STRING) && !is_token(_token, _VAR)) {
    assert_error(ERR_WRONG_TOKEN);
    return "";
  }
  return string(_token->_data->_string);
}

Generator::Generator() { this->_is_local = 0; }

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
  this->_variables = set<string>();
  this->_code = "";
  this->_main_code = this->generate_module(_ast);
  return this->generate_header() + this->_code + "int main() {\n" +
         this->generate_variables() + this->_main_code + "\n return 0;}";
}

string Generator::generate_header() {
  return "#include <spy/core/object.hpp>\n#include "
         "<spy/core/spy_standard.hpp>\n";
}

string Generator::generate_variables() {
  string _code = "";
  for (set<string>::iterator it = this->_variables.begin();
       it != this->_variables.end(); ++it)
    _code += "_object* " + *it + ";\n";
  return _code;
}

string Generator::generate_module(AST *_ast) {
  if (!is_ast(_ast, AST_MODULE)) {
    assert_error(ERR_WRONG_AST);
  }
  return this->generate_stmts(_ast->_first_child);
}

string Generator::generate_stmts(AST *_ast) {
  string _code = "";
  string _tmp;
  if (!is_ast(_ast, AST_STMTS)) {
    cout << "stmts " << endl;
    assert_error(ERR_WRONG_AST);
  }
  for (AST *_dummy = _ast->_first_child; _dummy != NULL;
       _dummy = _dummy->_next) {
    _tmp = this->generate_stmt(_dummy);
    _code.append(_tmp);
  }
  return _code;
}

string Generator::generate_stmt(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_STMT)) {
    cout << "stmt " << endl;
    assert_error(ERR_WRONG_AST);
  }
  if (is_ast(_ast->_first_child, AST_ASSIGN)) {
    _code += this->generate_assign(_ast->_first_child);
  } else if (is_ast(_ast->_first_child, AST_IF))
    _code += this->generate_if(_ast->_first_child);
  else if (is_ast(_ast->_first_child, AST_EXPR))
    _code += this->generate_expr(_ast->_first_child);
  else if (is_ast(_ast->_first_child, AST_FUNCTIONDEF)) {
    _code += this->generate_functiondef(_ast->_first_child);
  } else if (is_ast(_ast->_first_child, AST_RETURN))
    _code += this->generate_return(_ast->_first_child);
  else {
    assert_error(ERR_WRONG_AST);
  }
  _code += ";\n";
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
    /////////////////////////////////////////
    if (this->_is_local)
      this->_local_variables.insert(_code);
    else
      this->_variables.insert(_code);
    /////////////////////////////////////////
    _code += " = ";
    _code += this->generate_expr(_expr2);
  } else
    assert_error(ERR_NOT_IMPLEMENTED);
  return _code;
}

string Generator::generate_expr(AST *_ast) {
  string _code = "";
  if (_ast == NULL)
    return _code;
  if (!is_expr(_ast)) {
    cout << "expr " << endl;
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
  else if (is_ast(_ast, AST_BOOLOP))
    _code += this->generate_boolop(_ast);
  else if (is_ast(_ast, AST_CMPOP))
    _code += this->generate_cmpop(_ast);
  else if (is_ast(_ast, AST_CALL))
    _code += this->generate_call(_ast);
  else if (is_ast(_ast, AST_LIST))
    _code += this->generate_list(_ast);
  else
    assert_error(ERR_NOT_IMPLEMENTED);

  return _code;
}

string Generator::generate_identifier(AST *_ast) {
  if (!is_ast(_ast, AST_IDENTIFIER)) {
    cout << "id " << endl;
    assert_error(ERR_WRONG_AST);
  }
  return get_string_from_token(_ast->_token);
}

string Generator::generate_constant(AST *_ast) {
  string _code = "";
  if (!is_ast(_ast, AST_CONSTANT)) {
    cout << "const " << endl;
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
    cout << "binop " << endl;
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
    cout << "boolop " << endl;
    assert_error(ERR_WRONG_AST);
  }
  AST *_expr = _ast->_first_child;
  AST *_expr1 = _expr->_first_child;
  AST *_expr2 = _expr1 == NULL ? NULL : _expr1->_next;
  string _code = "";

  _code = "_object(" + this->generate_expr(_expr1) + "," +
          this->generate_expr(_expr2) + ")";
  if (is_ast(_expr, AST_AND)) {
    _code = "and" + _code;
  } else if (is_ast(_expr, AST_OR)) {
    _code = "or" + _code;
  } else
    assert_error(ERR_WRONG_TOKEN);

  return _code;
}

string Generator::generate_cmpop(AST *_ast) {
  if (!is_ast(_ast, AST_CMPOP)) {
    cout << "cmpop" << endl;
    assert_error(ERR_WRONG_AST);
  }
  AST *_expr = _ast->_first_child;
  AST *_expr1 = _expr->_first_child;
  AST *_expr2 = _expr1 == NULL ? NULL : _expr1->_next;
  string _code = "";

  _code = "_object(" + this->generate_expr(_expr1) + "," +
          this->generate_expr(_expr2) + ")";
  if (is_ast(_expr, AST_EQ)) {
    _code = "eq" + _code;
  } else if (is_ast(_expr, AST_LT)) {
    _code = "lt" + _code;
  } else if (is_ast(_expr, AST_LTE)) {
    _code = "lte" + _code;
  } else if (is_ast(_expr, AST_GT)) {
    _code = "gt" + _code;
  } else if (is_ast(_expr, AST_GTE)) {
    _code = "gte" + _code;
  } else if (is_ast(_expr, AST_IN)) {
    _code = "in" + _code;
  } else {
    assert_error(ERR_WRONG_TOKEN);
  }

  return _code;
}

string Generator::generate_if(AST *_ast) {
  if (!is_ast(_ast, AST_IF)) {
    cout << "if" << endl;
    assert_error(ERR_WRONG_AST);
  }
  string _code = "";
  string _condition = "";
  string _body = "";
  string _else = "";
  AST *_expr_condition = _ast->_first_child;
  AST *_stmts_body = _expr_condition->_next;
  AST *_else_stmt = _stmts_body->_next;

  _condition = "get_integer(" + this->generate_expr(_expr_condition) + ")";
  _body = this->generate_stmts(_stmts_body);
  _else =
      _else_stmt == NULL ? "" : this->generate_stmts(_else_stmt->_first_child);
  _code += "if (" + _condition + ") {\n " + _body + "} else {\n" + _else + "}";
  return _code;
}

string Generator::generate_functiondef(AST *_ast) {
  if (!is_ast(_ast, AST_FUNCTIONDEF)) {
    assert_error(ERR_WRONG_AST);
  }

  string _code = "";
  string _fun_id = "";
  string _args = "";
  string _stmts = "";

  AST *_expr_fun_id = _ast->_first_child;
  AST *_expr_args = _expr_fun_id->_next;
  AST *_stmts_body = _expr_args->_next;

  _fun_id = get_string_from_token(_expr_fun_id->_token);
  for (AST *_dummy = _expr_args->_first_child; _dummy != NULL;
       _dummy = _dummy->_next) {
    _args += "_object *" + get_string_from_token(_dummy->_token);
    if (_dummy->_next != NULL)
      _args += ",";
  }
  this->_is_local = 1;
  _stmts = this->generate_stmts(_stmts_body);
  this->_is_local = 0;
  string _lv = "";
  for (set<string>::iterator it = this->_local_variables.begin();
       it != this->_local_variables.end(); ++it)
    _lv += "_object* " + *it + ";\n";
  _code += "_object* " + _fun_id + "(" + _args + ") {\n" + _lv + _stmts + "}\n";
  this->_code += _code;
  return "";
  // return _code;
}

string Generator::generate_return(AST *_ast) {
  if (!is_ast(_ast, AST_RETURN)) {
    assert_error(ERR_WRONG_AST);
  }

  string _code = "";
  _code += "return ";
  _code += this->generate_expr(_ast->_first_child);
  return _code;
}

string Generator::generate_call(AST *_ast) {
  if (!is_ast(_ast, AST_CALL)) {
    assert_error(ERR_WRONG_AST);
  }
  string _code = "";
  string _fun_id = "";
  string _exprs = "";

  AST *_expr_fun_id = _ast->_first_child;
  AST *_exprs_args = _expr_fun_id->_next;

  _fun_id = get_string_from_token(_expr_fun_id->_token);
  if (_fun_id == "str" || _fun_id == "input" || _fun_id == "int" ||
      _fun_id == "float" || _fun_id == "print") {
    _fun_id += "_object";
  }
  for (AST *_dummy = _exprs_args->_first_child; _dummy != NULL;
       _dummy = _dummy->_next) {
    _exprs += this->generate_expr(_dummy);
    if (_dummy->_next != NULL)
      _exprs += ",";
  }

  _code += _fun_id + "(" + _exprs + ")";
  return _code;
}

string Generator::generate_list(AST *_ast) {
  if (!is_ast(_ast, AST_LIST))
    assert_error(ERR_WRONG_AST);
  AST *_exprs_ast = _ast->_first_child;
  string _code = "";
  if (!is_ast(_ast, AST_EXPRS))
    assert_error(ERR_WRONG_AST);
  // FIXME
  for (AST *_dummy = _exprs_ast->_first_child; _dummy != NULL;
       _dummy = _dummy->_next) {
  }
}
