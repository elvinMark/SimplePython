#include <spy/generator/parser.hpp>

Parser::Parser(string raw_code) {
  this->raw_code = raw_code;
  this->token = NULL;
  this->curr_token = NULL;
}

void Parser::generate_tokens() {
  TOKEN *token = create_token(_START, null_data, NULL);
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

      else if (tmp == "from")
        token->_next_token = create_token(_FROM, null_data, NULL);

      else if (tmp == "pass")
        token->_next_token = create_token(_PASS, null_data, NULL);

      else if (tmp == "break")
        token->_next_token = create_token(_BREAK, null_data, NULL);

      else if (tmp == "continue")
        token->_next_token = create_token(_CONTINUE, null_data, NULL);

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
      } else if ((curr_idx + 1) < length &&
                 this->raw_code[curr_idx + 1] == '<') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_LTE, null_data, NULL);
      } else if ((curr_idx + 1) < length &&
                 this->raw_code[curr_idx + 1] == '>') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_GTE, null_data, NULL);
      } else
        token->_next_token = create_token(_ASSIGN, null_data, NULL);

      curr_idx += 1;
      token = token->_next_token;
    }

    else if (curr_chr == '<') {
      if ((curr_idx + 1) < length && this->raw_code[curr_idx + 1] == '<') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_SHIFT_LEFT, null_data, NULL);
      } else if ((curr_idx + 1) < length &&
                 this->raw_code[curr_idx + 1] == '=') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_LTE, null_data, NULL);
      } else
        token->_next_token = create_token(_OP_LT, null_data, NULL);

      curr_idx += 1;
      token = token->_next_token;
    }

    else if (curr_chr == '>') {
      if ((curr_idx + 1) < length && this->raw_code[curr_idx + 1] == '>') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_SHIFT_RIGHT, null_data, NULL);
      } else if ((curr_idx + 1) < length &&
                 this->raw_code[curr_idx + 1] == '=') {
        curr_idx += 1;
        token->_next_token = create_token(_OP_GTE, null_data, NULL);
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
      token->_next_token = create_token(_ENDL, null_data, NULL);
      token = token->_next_token;
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

  token->_next_token = create_token(_ENDL, null_data, NULL);
  token = token->_next_token;
  token->_next_token = create_token(_END, null_data, NULL);
}

void Parser::consume_token(int _token) {
  if (this->curr_token->_type != _token)
    assert_error(ERR_WRONG_TOKEN);
  this->curr_token = this->curr_token->_next_token;
}

int Parser::peek_token(int i) {
  TOKEN *dummy_;
  dummy_ = this->curr_token;
  while (i-- > 0 && dummy_ != NULL)
    dummy_ = dummy_->_next_token;
  return dummy_ == NULL ? -1 : dummy_->_type;
}

int Parser::is_token(int _token) { return this->curr_token->_type == _token; }

AST *Parser::generate_ast() {
  if (this->token == NULL)
    assert_error(ERR_NO_TOKENS);

  this->curr_token = this->token;

  AST *ast = new AST(AST_MODULE, NULL);

  this->consume_token(_START);
  ast->add_child(this->parse_stmts());
  this->consume_token(_END);

  this->curr_token = this->token;

  return ast;
}

AST *Parser::parse() {
  this->generate_tokens();
  return this->generate_ast();
}

AST *Parser::parse_stmt() {
  // TODO
  AST *ast = new AST(AST_STMT, NULL);

  if (this->is_token(_DEF))
    ast->add_child(this->parse_functiondef());

  else if (this->is_token(_CLASS))
    ast->add_child(this->parse_classdef());

  else if (this->is_token(_RETURN))
    ast->add_child(this->parse_return());

  else if (this->is_token(_FOR))
    ast->add_child(this->parse_for());

  else if (this->is_token(_WHILE))
    ast->add_child(this->parse_while());

  else if (this->is_token(_IF))
    ast->add_child(this->parse_if());

  else if (this->is_token(_IMPORT))
    ast->add_child(this->parse_import());

  else if (this->is_token(_FROM))
    ast->add_child(this->parse_importfrom());

  else if (this->is_token(_PASS))
    ast->add_child(this->parse_pass());

  else if (this->is_token(_BREAK))
    ast->add_child(this->parse_break());

  else if (this->is_token(_CONTINUE))
    ast->add_child(this->parse_continue());

  else
    ast->add_child(this->parse_expr());

  return ast;
}

AST *Parser::parse_stmts() {
  // TODO
  AST *ast = new AST(AST_STMTS, NULL);

  while (!this->is_token(_END)) {
    ast->add_child(this->parse_stmt());
    while (this->is_token(_ENDL))
      this->curr_token = this->curr_token->_next_token;
  }

  return ast;
}

AST *Parser::parse_expr() {
  // TODO
  AST *ast = new AST(AST_EXPR, NULL);
  TOKEN *tmp_;
  int _op = this->peek_token(1);

  if (this->is_token(_VAR)) {

  } else if (this->is_token(_DATA_INTEGER) | this->is_token(_DATA_REAL) |
             this->is_token(_DATA_STRING)) {
    ast->add_child(this->parse_constant());
  } else if (this->is_token(_OPEN_SQUARE_BRACKET)) {
    ast->add_child(this->parse_list());
  } else if (this->is_token(_OPEN_CURLY_BRACKET)) {
    ast->add_child(this->parse_dict());
  } else if (this->is_token(_LAMBDA)) {
    ast->add_child(this->parse_lambda());
  } else if (this->is_token(_VAR) & _op == _OPEN_ROUND_BRACKET) {
    ast->add_child(this->parse_call());
  } else if (this->is_token(_VAR) & _op == _OPEN_SQUARE_BRACKET) {
    ast->add_child(this->parse_subscript());
  }
  return ast;
}

AST *Parser::parse_identifier() {
  AST *ast = new AST(AST_IDENTIFIER, this->curr_token);
  this->consume_token(_VAR);
  return ast;
}

AST *Parser::parse_constant() {
  if (!(this->is_token(_DATA_INTEGER) | this->is_token(_DATA_REAL) |
        this->is_token(_DATA_STRING)))
    assert_error(ERR_EXPECT_CONSTANT);

  AST *ast = new AST(AST_CONSTANT, this->curr_token);
  this->curr_token = this->curr_token->_next_token;
  return ast;
}

AST *Parser::parse_assign() {
  AST *ast = new AST(AST_ASSIGN, NULL);
  ast->add_child(this->parse_expr());

  // Consume =
  this->consume_token(_ASSIGN);

  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_classdef() {
  AST *ast = new AST(AST_CLASSDEF, NULL);
  ast->add_child(this->parse_identifier());

  // Consume :
  this->consume_token(_COLON);
  // Consume \n
  this->consume_token(_ENDL);
  // Consume start of class
  this->consume_token(_START);

  return ast;
}

AST *Parser::parse_functiondef() {
  AST *ast = new AST(AST_FUNCTIONDEF, NULL);

  ast->add_child(this->parse_identifier());

  // Consume (
  this->consume_token(_OPEN_ROUND_BRACKET);

  ast->add_child(this->parse_arguments());

  // Consume )
  this->consume_token(_CLOSE_ROUND_BRACKET);
  // Consume :
  this->consume_token(_COLON);
  // Consume start
  this->consume_token(_START);

  ast->add_child(this->parse_stmts());

  this->consume_token(_END);

  return ast;
}

AST *Parser::parse_arguments() {
  AST *ast = new AST(AST_ARGUMENTS, NULL);

  while (!this->is_token(_CLOSE_ROUND_BRACKET)) {
    ast->add_child(this->parse_identifier());
    if (this->is_token(_COMMA))
      this->consume_token(_COMMA);
  }

  return ast;
}

AST *Parser::parse_return() {
  AST *ast = new AST(AST_RETURN, NULL);
  // consume return
  this->consume_token(_RETURN);
  ast->add_child(this->parse_expr());

  return ast;
}

AST *Parser::parse_if() {
  AST *ast = new AST(AST_IF, NULL);
  // consume if
  this->consume_token(_IF);
  ast->add_child(this->parse_expr());
  // consume : and start
  this->consume_token(_COLON);
  this->consume_token(_START);
  // consume statements
  ast->add_child(this->parse_stmts());
  // consume end of if
  this->consume_token(_END);

  if (this->is_token(_ELIF)) {
    this->consume_token(_ELIF);
    TOKEN *token_if = create_token(_IF, null_data, this->curr_token);
    TOKEN *token_else = create_token(_ELSE, null_data, token_if);
    this->curr_token = token_else;
    ast->add_child(this->parse_else());
  }

  else if (this->is_token(_ELSE))
    ast->add_child(this->parse_else());

  return ast;
}

AST *Parser::parse_else() {
  AST *ast = new AST(AST_ELSE, NULL);
  // consume else and :
  this->consume_token(_ELSE);
  this->consume_token(_COLON);
  // consume start, parse statements and cosume end
  this->consume_token(_START);
  ast->add_child(this->parse_stmts());
  this->consume_token(_END);
  return ast;
}

AST *Parser::parse_initerator() {
  AST *ast = new AST(AST_INITERATOR, NULL);
  ast->add_child(this->parse_identifier());
  this->consume_token(_IN);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_for() {
  AST *ast = new AST(AST_FOR, NULL);
  this->consume_token(_FOR);
  ast->add_child(this->parse_initerator());
  this->consume_token(_COLON);
  this->consume_token(_START);
  ast->add_child(this->parse_stmts());
  this->consume_token(_END);
  return ast;
}

AST *Parser::parse_while() {
  AST *ast = new AST(AST_WHILE, NULL);
  this->consume_token(_WHILE);
  ast->add_child(this->parse_expr());
  this->consume_token(_COLON);
  this->consume_token(_START);
  ast->add_child(this->parse_stmts());
  this->consume_token(_END);
  return ast;
}

AST *Parser::parse_import() {
  AST *ast = new AST(AST_IMPORT, NULL);
  this->consume_token(_IMPORT);
  ast->add_child(this->parse_identifier());
  return ast;
}

AST *Parser::parse_importfrom() {
  AST *ast = new AST(AST_IMPORTFROM, NULL);
  this->consume_token(_FROM);
  ast->add_child(this->parse_identifier());
  this->consume_token(_IMPORT);
  ast->add_child(this->parse_identifier());
  return ast;
}

AST *Parser::parse_pass() {
  AST *ast = new AST(AST_PASS, NULL);
  this->consume_token(_PASS);
  return ast;
}

AST *Parser::parse_break() {
  AST *ast = new AST(AST_BREAK, NULL);
  this->consume_token(_BREAK);
  return ast;
}

AST *Parser::parse_continue() {
  AST *ast = new AST(AST_CONTINUE, NULL);
  this->consume_token(_CONTINUE);
  return ast;
}

AST *Parser::parse_list() {
  AST *ast = new AST(AST_LIST, NULL);
  this->consume_token(_OPEN_SQUARE_BRACKET);
  while (!this->is_token(_CLOSE_SQUARE_BRACKET)) {
    ast->add_child(this->parse_expr());
    if (this->is_token(_COMMA))
      this->consume_token(_COMMA);
  }
  this->consume_token(_CLOSE_SQUARE_BRACKET);
  return ast;
}

AST *Parser::parse_dict() {
  AST *ast = new AST(AST_DICT, NULL);
  AST *ast_keys = new AST(AST_EXPRS, NULL);
  AST *ast_values = new AST(AST_EXPRS, NULL);

  this->consume_token(_OPEN_CURLY_BRACKET);
  while (!this->is_token(_CLOSE_CURLY_BRACKET)) {
    ast_keys->add_child(this->parse_expr());
    this->consume_token(_COLON);
    ast_values->add_child(this->parse_expr());
    if (this->is_token(_COMMA))
      this->consume_token(_COMMA);
  }
  this->consume_token(_CLOSE_CURLY_BRACKET);
  ast->add_child(ast_keys);
  ast->add_child(ast_values);

  return ast;
}

AST *Parser::parse_call() {
  AST *ast = new AST(AST_CALL, NULL);
  AST *ast_args = new AST(AST_EXPRS, NULL);
  ast->add_child(this->parse_identifier());
  this->consume_token(_OPEN_ROUND_BRACKET);
  while (!this->is_token(_CLOSE_ROUND_BRACKET)) {
    ast_args->add_child(this->parse_expr());
    if (this->is_token(_COMMA))
      this->consume_token(_COMMA);
  }
  this->consume_token(_CLOSE_ROUND_BRACKET);
  ast->add_child(ast_args);
  return ast;
}

AST *Parser::parse_subscript() {
  AST *ast = new AST(AST_SUBSCRIPT, NULL);
  ast->add_child(this->parse_identifier());
  this->consume_token(_OPEN_SQUARE_BRACKET);
  ast->add_child(this->parse_slice());
  this->consume_token(_CLOSE_SQUARE_BRACKET);
  return ast;
}

AST *Parser::parse_slice() {
  AST *ast = new AST(AST_SLICE, NULL);
  AST *low = NULL;
  AST *high = NULL;
  AST *step = NULL;

  low = this->parse_expr();
  if (this->is_token(_COLON)) {
    this->consume_token(_COLON);
    high = this->parse_expr();
  }

  if (this->is_token(_COLON)) {
    this->consume_token(_COLON);
    step = this->parse_expr();
  }

  ast->add_child(low);
  ast->add_child(high);
  ast->add_child(step);

  return ast;
}

AST *Parser::parse_boolop() {
  AST *ast = new AST(AST_BOOLOP, NULL);
  int _op = this->peek_token(1);
  if (_op == _AND)
    ast->add_child(this->parse_and());
  else if (_op == _OR)
    ast->add_child(this->parse_or());
  else
    assert_error(ERR_WRONG_TOKEN);
  return ast;
}

AST *Parser::parse_binop() {
  AST *ast = new AST(AST_BINOP, NULL);
  int _op = this->peek_token(1);

  if (_op == _OP_ADD)
    ast->add_child(this->parse_add());
  else if (_op == _OP_DIF)
    ast->add_child(this->parse_sub());
  else if (_op == _OP_MUL)
    ast->add_child(this->parse_mult());
  else if (_op == _OP_DIV)
    ast->add_child(this->parse_div());
  else if (_op == _OP_MOD)
    ast->add_child(this->parse_mod());
  else if (_op == _OP_POW)
    ast->add_child(this->parse_pow());
  else if (_op == _OP_SHIFT_LEFT)
    ast->add_child(this->parse_lshift());
  else if (_op == _OP_SHIFT_RIGHT)
    ast->add_child(this->parse_rshift());
  else if (_op == _OP_OR)
    ast->add_child(this->parse_bitor());
  else if (_op == _OP_AND)
    ast->add_child(this->parse_bitand());
  else if (_op == _OP_XOR)
    ast->add_child(this->parse_bitxor());

  return ast;
}

AST *Parser::parse_unaryop() {
  AST *ast = new AST(AST_UNARYOP, NULL);
  ast->add_child(this->parse_not());
  return ast;
}

AST *Parser::parse_cmpop() {
  AST *ast = new AST(AST_CMPOP, NULL);
  int _op = this->peek_token(1);

  if (_op == _OP_EQUAL)
    ast->add_child(this->parse_eq());
  else if (_op == _OP_LT)
    ast->add_child(this->parse_lt());
  else if (_op == _OP_LTE)
    ast->add_child(this->parse_lte());
  else if (_op == _OP_GT)
    ast->add_child(this->parse_gt());
  else if (_op == _OP_GTE)
    ast->add_child(this->parse_gte());
  else if (_op == _IN)
    ast->add_child(this->parse_in());
  else
    assert_error(ERR_WRONG_TOKEN);

  return ast;
}

AST *Parser::parse_ifexpr() {
  AST *ast = new AST(AST_IFEXPR, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_IF);
  ast->add_child(this->parse_expr());
  this->consume_token(_ELSE);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_lambda() {
  AST *ast = new AST(AST_LAMBDA, NULL);
  this->consume_token(_LAMBDA);
  ast->add_child(this->parse_arguments());
  this->consume_token(_COLON);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_and() {
  AST *ast = new AST(AST_AND, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_AND);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_or() {
  AST *ast = new AST(AST_OR, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OR);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_add() {
  AST *ast = new AST(AST_ADD, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_ADD);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_sub() {
  AST *ast = new AST(AST_SUB, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_DIF);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_mult() {
  AST *ast = new AST(AST_MULT, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_MUL);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_div() {
  AST *ast = new AST(AST_DIV, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_DIV);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_mod() {
  AST *ast = new AST(AST_MOD, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_MOD);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_pow() {
  AST *ast = new AST(AST_POW, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_POW);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_lshift() {
  AST *ast = new AST(AST_LSHIFT, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_SHIFT_LEFT);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_rshift() {
  AST *ast = new AST(AST_RSHIFT, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_SHIFT_RIGHT);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_bitor() {
  AST *ast = new AST(AST_BITOR, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_OR);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_bitand() {
  AST *ast = new AST(AST_BITAND, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_AND);
  ast->add_child(this->parse_expr());
  return ast;
}
AST *Parser::parse_bitxor() {
  AST *ast = new AST(AST_BITXOR, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_OR);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_not() {
  AST *ast = new AST(AST_NOT, NULL);
  this->consume_token(_NOT);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_eq() {
  AST *ast = new AST(AST_EQ, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_EQUAL);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_lt() {
  AST *ast = new AST(AST_LT, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_LT);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_lte() {
  AST *ast = new AST(AST_LTE, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_LTE);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_gt() {
  AST *ast = new AST(AST_GT, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_GT);
  ast->add_child(this->parse_expr());
  return ast;
}

AST *Parser::parse_gte() {
  AST *ast = new AST(AST_GTE, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_OP_GTE);
  ast->add_child(this->parse_expr());
  return ast;
}
AST *Parser::parse_in() {
  AST *ast = new AST(AST_IN, NULL);
  ast->add_child(this->parse_expr());
  this->consume_token(_IN);
  ast->add_child(this->parse_expr());
  return ast;
}

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
