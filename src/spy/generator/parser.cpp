#include <spy/generator/parser.hpp>

Parser::Parser(string raw_code) {
  this->raw_code = raw_code;
  this->token = NULL;
  this->curr_token = NULL;
}

void Parser::generate_tokens() {
  TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
  int curr_idx = 0;
  char token_map[128];
  char curr_chr;
  int length = this->raw_code.length();
  int isreal;
  string tmp;
  int flag_newline = 1;
  int spaces;
  int depth = 0;
  int new_depth;
  int skip = 0;

  this->token = token;
  create_map_characters(token_map);

  while (curr_idx < length) {

    if (skip) {
      while (curr_idx < length && this->raw_code[curr_idx] != '\n')
        curr_idx++;
      skip = 0;
    }

    if (flag_newline) {
      spaces = 0;

      while (curr_idx < length && this->raw_code[curr_idx] == ' ') {
        curr_idx++;
        spaces++;
      }

      if (spaces % SPACES_INDENTATION)
        assert_error(ERR_INDENTATION);

      else {
        new_depth = spaces / SPACES_INDENTATION;
        if (new_depth > depth) {

          if (new_depth - depth > 1)
            assert_error(ERR_INDENTATION);

          token->_next_token = create_token(_START, null_data, NULL);
          token = token->_next_token;
          depth = new_depth;
        }

        else if (new_depth < depth) {
          for (; depth > new_depth; depth--) {
            token->_next_token = create_token(_END, null_data, NULL);
            token = token->_next_token;
          }
        }
      }

      flag_newline = 0;
    }

    curr_chr = this->raw_code[curr_idx];

    if (is_symbol(curr_chr) && token_map[curr_chr] != 0) {
      token->_next_token = create_token(token_map[curr_chr], null_data, NULL);
      token = token->_next_token;
      curr_idx += 1;
    }

    else if (is_number(curr_chr)) {
      tmp = "";
      isreal = 0;
      DATA d_;

      for (; curr_idx < length; curr_idx++) {
        curr_chr = this->raw_code[curr_idx];
        if (is_number(curr_chr) || curr_chr == '.') {
          if (curr_chr == '.')
            isreal = 1;
          tmp.push_back(curr_chr);
        } else
          break;
      }

      if (isreal) {
        d_._real = stod(tmp);
        token->_next_token = create_token(_DATA_REAL, d_, NULL);
      } else {
        d_._integer = stoi(tmp);
        token->_next_token = create_token(_DATA_INTEGER, d_, NULL);
      }

      token = token->_next_token;
    }

    else if (curr_chr == '"') {
      tmp = "";
      DATA d_;

      curr_idx += 1;
      for (; curr_idx < length; curr_idx++) {
        curr_chr = this->raw_code[curr_idx];
        if (curr_chr == '"') {
          curr_idx += 1;
          break;
        }
        tmp.push_back(curr_chr);
      }
      d_._string = new char[tmp.length() + 1];
      strcpy(d_._string, tmp.c_str());
      token->_next_token = create_token(_DATA_STRING, d_, NULL);
      token = token->_next_token;
    }

    else if (is_alphabet(curr_chr)) {
      tmp = "";
      DATA d_;

      for (; curr_idx < length; curr_idx++) {
        curr_chr = this->raw_code[curr_idx];
        if (is_alphabet(curr_chr) || curr_chr == '_' || is_number(curr_chr))
          tmp.push_back(curr_chr);
        else
          break;
      }

      if (tmp == "if")
        token->_next_token = create_token(_IF, null_data, NULL);

      else if (tmp == "else")
        token->_next_token = create_token(_ELSE, null_data, NULL);

      else if (tmp == "elif")
        token->_next_token = create_token(_ELIF, null_data, NULL);

      else if (tmp == "while")
        token->_next_token = create_token(_WHILE, null_data, NULL);

      else if (tmp == "for")
        token->_next_token = create_token(_FOR, null_data, NULL);

      else if (tmp == "in")
        token->_next_token = create_token(_IN, null_data, NULL);

      else if (tmp == "return")
        token->_next_token = create_token(_RETURN, null_data, NULL);

      else if (tmp == "lambda")
        token->_next_token = create_token(_LAMBDA, null_data, NULL);

      else if (tmp == "and")
        token->_next_token = create_token(_AND, null_data, NULL);

      else if (tmp == "or")
        token->_next_token = create_token(_OR, null_data, NULL);

      else if (tmp == "not")
        token->_next_token = create_token(_NOT, null_data, NULL);

      else if (tmp == "def")
        token->_next_token = create_token(_DEF, null_data, NULL);

      else if (tmp == "class")
        token->_next_token = create_token(_CLASS, null_data, NULL);

      else if (tmp == "True")
        token->_next_token = create_token(_TRUE, null_data, NULL);

      else if (tmp == "False")
        token->_next_token = create_token(_FALSE, null_data, NULL);

      else if (tmp == "import")
        token->_next_token = create_token(_IMPORT, null_data, NULL);

      else if (tmp == "with")
        token->_next_token = create_token(_WITH, null_data, NULL);

      else if (tmp == "as")
        token->_next_token = create_token(_AS, null_data, NULL);

      else if (tmp == "try")
        token->_next_token = create_token(_TRY, null_data, NULL);

      else if (tmp == "except")
        token->_next_token = create_token(_EXCEPT, null_data, NULL);

      else if (tmp == "raise")
        token->_next_token = create_token(_RAISE, null_data, NULL);

      else if (tmp == "assert")
        token->_next_token = create_token(_ASSERT, null_data, NULL);

      else if (tmp == "pass")
        token->_next_token = create_token(_PASS, null_data, NULL);

      else if (tmp == "break")
        token->_next_token = create_token(_BREAK, null_data, NULL);

      else if (tmp == "continue")
        token->_next_token = create_token(_CONTINUE, null_data, NULL);

      else if (tmp == "global")
        token->_next_token = create_token(_GLOBAL, null_data, NULL);

      else {
        d_._string = new char[tmp.length() + 1];
        strcpy(d_._string, tmp.c_str());
        token->_next_token = create_token(_VAR, d_, NULL);
      }

      token = token->_next_token;
    }

    else if (curr_chr == '=') {
      if ((curr_idx + 1) < length && this->raw_code[curr_idx + 1] == '=') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_EQUAL, null_data, NULL);
      } else
        token->_next_token = create_token(_ASSIGN, null_data, NULL);

      curr_idx += 1;
      token = token->_next_token;
    }

    else if (curr_chr == '<') {
      if ((curr_idx + 1) < length && this->raw_code[curr_idx + 1] == '<') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_SHIFT_LEFT, null_data, NULL);
      } else
        token->_next_token = create_token(_OP_LT, null_data, NULL);

      curr_idx += 1;
      token = token->_next_token;
    }

    else if (curr_chr == '>') {
      if ((curr_idx + 1) < length && this->raw_code[curr_idx + 1] == '>') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_SHIFT_RIGHT, null_data, NULL);
      } else
        token->_next_token = create_token(_OP_GT, null_data, NULL);

      curr_idx += 1;
      token = token->_next_token;
    }

    else if (curr_chr == '*') {
      if ((curr_idx + 1) < length && this->raw_code[curr_idx + 1] == '*') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_POW, null_data, NULL);
      } else
        token->_next_token = create_token(_OP_MUL, null_data, NULL);

      curr_idx += 1;
      token = token->_next_token;
    }

    else if (curr_chr == '\n') {
      curr_idx += 1;
      flag_newline = 1;
    }

    else if (curr_chr == ' ')
      curr_idx += 1;

    else if (curr_chr == '#') {
      skip = 1;
      curr_idx += 1;
    }

    else
      curr_idx += 1;
  }

  this->token = this->token->_next_token;
}

AST *Parser::generate_ast() {
  if (this->token == NULL)
    assert_error(ERR_NO_TOKENS);

  this->curr_token = this->token;
  AST *ast = new AST(AST_MODULE, NULL);

  while (this->curr_token != NULL)
    ast->add_child(this->parser_stmt());

  this->curr_token = this->token;

  return ast;
}

AST *Parser::parse() {
  this->generate_tokens();
  return this->generate_ast();
}

AST *Parser::parse_stmt() {
  AST *ast = new AST(AST_STMT, NULL);

  if (this->curr_token->_type == _DEF)
    ast->add_child(this->parse_functiondef());

  else if (this->curr_token->_type == _CLASS)
    ast->add_child(this->parse_classdef());

  else if (this->curr_token->_type == _RETURN)
    ast->add_child(this->parse_return());

  else if (this->curr_token->_type == _FOR)
    ast->add_child(this->parse_for());

  else if (this->curr_token->_type == _WHILE)
    ast->add_child(this->parse_while());

  else if (this->curr_token->_type == _IF)
    ast->add_child(this->parse_if());

  else if (this->curr_token->_type == _WITH)
    ast->add_child(this->parse_with());

  else if (this->curr_token->_type == _RAISE)
    ast->add_child(this->parse_raise());

  else if (this->curr_token->_type == _TRY)
    ast->add_child(this->parse_try());

  else if (this->curr_token->_type == _ASSERT)
    ast->add_child(this->parse_assert());

  else if (this->curr_token->_type == _IMPORT)
    ast->add_child(this->parse_import());

  else if (this->curr_token->_type == _GLOBAL)
    ast->add_child(this->parse_global());

  else if (this->curr_token->_type == _PASS)
    ast->add_child(this->parse_pass());

  else if (this->curr_token->_type == _BREAK)
    ast->add_child(this->parse_break());

  else if (this->curr_token->_type == _CONTINUE)
    ast->add_child(this->parse_continue());

  else if (is_expression(this->curr_token->_type))
    ast->add_child(this->parse_expr());

  return ast;
}

AST *Parser::parse_expr() {
  AST *ast = new AST(AST_EXPR, NULL);

  if (this->curr_token->_type == _LAMBDA)
    ast->add_child(this->parse_lambda());

  return ast;
}

AST *Parser::parse_functiondef() { return NULL; }
AST *Parser::parse_classdef() { return NULL; }
AST *Parser::parse_return() { return NULL; }
AST *Parser::parse_assign() { return NULL; }
AST *Parser::parse_augassign() { return NULL; }
AST *Parser::parse_for() { return NULL; }
AST *Parser::parse_while() { return NULL; }
AST *Parser::parse_if() { return NULL; }
AST *Parser::parse_with() { return NULL; }
AST *Parser::parse_raise() { return NULL; }
AST *Parser::parse_try() { return NULL; }
AST *Parser::parse_assert() { return NULL; }
AST *Parser::parse_import() { return NULL; }
AST *Parser::parse_importfrom() { return NULL; }
AST *Parser::parse_global() { return NULL; }
AST *Parser::parse_pass() { return NULL; }
AST *Parser::parse_break() { return NULL; }
AST *Parser::parse_continue() { return NULL; }
AST *Parser::parse_boolop() { return NULL; }
AST *Parser::parse_namedexpr() { return NULL; }
AST *Parser::parse_binop() { return NULL; }
AST *Parser::parse_unaryop() { return NULL; }
AST *Parser::parse_lambda() { return NULL; }
AST *Parser::parse_ifexpr() { return NULL; }
AST *Parser::parse_dict() { return NULL; }
AST *Parser::parse_set() { return NULL; }
AST *Parser::parse_listcomp() { return NULL; }
AST *Parser::parse_setcomp() { return NULL; }
AST *Parser::parse_dictcomp() { return NULL; }
AST *Parser::parse_generatorexpr() { return NULL; }
AST *Parser::parse_compare() { return NULL; }
AST *Parser::parse_call() { return NULL; }
AST *Parser::parse_joinedstr() { return NULL; }
AST *Parser::parse_constant() { return NULL; }

Parser *Parser::from_code_path(string code_path) {
  ifstream ifs;
  string raw_code = "";
  ifs.open(code_path);
  if (ifs.is_open())
    while (ifs)
      raw_code.push_back(ifs.get());

  ifs.close();
  return new Parser(raw_code);
}
