#include <spy/generator/parser.hpp>

DATA null_data;

TOKEN *create_token() { return create_token(_NONE, null_data, NULL); }

TOKEN *create_token(int _type, DATA _data, TOKEN *_token) {
  TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
  token->_type = _type;
  token->_data = _data;
  token->_next_token = _token;
  return token;
}

int is_number(char c) { return c <= '9' && c >= '0'; }

int is_alphabet(char c) {
  return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
}

int is_symbol(char c) {
  return ((c <= 47 && c >= 34) || (c <= 64 && c >= 58) ||
          (c <= 96 && c >= 91) || (c <= 126 && c >= 123)) &&
         c != '#';
}

void create_map_characters(char *token_map) {
  token_map['+'] = _OP_ADD;
  token_map['-'] = _OP_DIF;
  token_map['*'] = _OP_MUL;
  token_map['/'] = _OP_DIV;
  token_map['{'] = _OPEN_CURLY_BRACKET;
  token_map['}'] = _CLOSE_CURLY_BRACKET;
  token_map['['] = _OPEN_SQUARE_BRACKET;
  token_map[']'] = _CLOSE_SQUARE_BRACKET;
  token_map['('] = _OPEN_ROUND_BRACKET;
  token_map[')'] = _CLOSE_ROUND_BRACKET;
  token_map[':'] = _COLON;
  token_map[','] = _COMMA;
  token_map['.'] = _DOT;
  token_map['&'] = _OP_AND;
  token_map['|'] = _OP_OR;
  token_map['^'] = _OP_XOR;
  token_map['~'] = _OP_NOT;
  token_map['%'] = _OP_MOD;
}

Parser::Parser(string raw_code) {
  this->raw_code = raw_code;
  this->token = NULL;
}

void Parser::generate_tokens() {
  TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
  int curr_idx = 0;
  char token_map[128];
  char curr_chr, next_chr;
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
  // TODO
  if (this->token == NULL)
    assert_error(ERR_NO_TOKENS);

  return NULL;
}

AST *Parser::parse() {
  this->generate_tokens();
  return this->generate_ast();
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
