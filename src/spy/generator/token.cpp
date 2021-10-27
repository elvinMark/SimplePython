#include <spy/generator/token.hpp>

string token_map[56] = {"_EMPTY",
                        "_ENDL",
                        "_NONE",
                        "_DATA_INTEGER",
                        "_DATA_REAL",
                        "_DATA_STRING",
                        "_CLASS",
                        "_DEF",
                        "_IF",
                        "_ELSE",
                        "_ELIF",
                        "_WHILE",
                        "_FOR",
                        "_IN",
                        "_RETURN",
                        "_LAMBDA",
                        "_IMPORT",
                        "_FROM",
                        "_PASS",
                        "_BREAK",
                        "_CONTINUE",
                        "_AND",
                        "_OR",
                        "_NOT",
                        "_VAR",
                        "_TRUE",
                        "_FALSE",
                        "_OPEN_CURLY_BRACKET",
                        "_CLOSE_CURLY_BRACKET",
                        "_OPEN_ROUND_BRACKET",
                        "_CLOSE_ROUND_BRACKET",
                        "_OPEN_SQUARE_BRACKET",
                        "_CLOSE_SQUARE_BRACKET",
                        "_COLON",
                        "_COMMA",
                        "_DOT",
                        "_START",
                        "_END",
                        "_ASSIGN",
                        "_OP_ADD",
                        "_OP_DIF",
                        "_OP_MUL",
                        "_OP_DIV",
                        "_OP_POW",
                        "_OP_GT",
                        "_OP_LT",
                        "_OP_LTE",
                        "_OP_GTE",
                        "_OP_EQUAL",
                        "_OP_AND",
                        "_OP_OR",
                        "_OP_XOR",
                        "_OP_MOD",
                        "_OP_NOT",
                        "_OP_SHIFT_LEFT",
                        "_OP_SHIFT_RIGHT"};

DATA *create_data(int _integer) {
  DATA *_data = (DATA *)malloc(sizeof(DATA));
  _data->_integer = _integer;
  return _data;
}

DATA *create_data(double _real) {
  DATA *_data = (DATA *)malloc(sizeof(DATA));
  _data->_real = _real;
  return _data;
}

DATA *create_data(string _string) {
  DATA *_data = (DATA *)malloc(sizeof(DATA));
  _data->_string = str_to_chr(_string);
  return _data;
}

TOKEN *create_token(int _type, DATA *_data, TOKEN *_token) {
  TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
  token->_type = _type;
  token->_data = _data;
  token->_next = _token;
  return token;
}

int is_number(char c) { return c <= '9' && c >= '0'; }

int is_alphabet(char c) {
  return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
}

int is_symbol(char c) {
  return ((c <= 47 && c >= 34) || (c <= 64 && c >= 58) ||
          (c <= 96 && c >= 91) || (c <= 126 && c >= 123));
}

void create_token_dict(char *token_dict) {
  token_dict['+'] = _OP_ADD;
  token_dict['-'] = _OP_DIF;
  token_dict['/'] = _OP_DIV;
  token_dict['{'] = _OPEN_CURLY_BRACKET;
  token_dict['}'] = _CLOSE_CURLY_BRACKET;
  token_dict['['] = _OPEN_SQUARE_BRACKET;
  token_dict[']'] = _CLOSE_SQUARE_BRACKET;
  token_dict['('] = _OPEN_ROUND_BRACKET;
  token_dict[')'] = _CLOSE_ROUND_BRACKET;
  token_dict[':'] = _COLON;
  token_dict[','] = _COMMA;
  token_dict['.'] = _DOT;
  token_dict['&'] = _OP_AND;
  token_dict['|'] = _OP_OR;
  token_dict['^'] = _OP_XOR;
  token_dict['~'] = _OP_NOT;
  token_dict['%'] = _OP_MOD;
}

TOKEN *tokenize(string s) {
  TOKEN *token = create_token(_START, NULL, NULL);
  TOKEN *output;
  int prev_idx;
  int curr_idx = 0;
  char token_dict[128];
  char curr_chr;
  int length = s.length();
  int isreal;
  string _str;
  int flag_newline = 1;
  int spaces;
  int depth = 0;
  int new_depth;
  int skip = 0;

  output = token;
  create_token_dict(token_dict);

  while (curr_idx < length) {
    if (skip) {
      while (curr_idx < length && s[curr_idx] != '\n')
        curr_idx++;
      skip = 0;
    }

    if (flag_newline) {
      spaces = 0;

      while (curr_idx < length && s[curr_idx] == ' ') {
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

          token->_next = create_token(_START, NULL, NULL);
          token = token->_next;
          depth = new_depth;
        }

        else if (new_depth < depth) {
          for (; depth > new_depth; depth--) {
            token->_next = create_token(_END, NULL, NULL);
            token = token->_next;
          }
        }
      }

      flag_newline = 0;
    }

    curr_chr = s[curr_idx];

    if (is_symbol(curr_chr) && token_dict[curr_chr] != 0) {
      token->_next = create_token(token_dict[curr_chr], NULL, NULL);
      token = token->_next;
      curr_idx += 1;
    }

    else if (is_number(curr_chr)) {
      isreal = 0;
      prev_idx = curr_idx;

      for (; curr_idx < length; curr_idx++) {
        curr_chr = s[curr_idx];
        if (is_number(curr_chr) || curr_chr == '.') {
          if (curr_chr == '.')
            isreal = 1;
        } else
          break;
      }

      if (isreal) {
        token->_next = create_token(
            _DATA_REAL,
            create_data(stod(s.substr(prev_idx, curr_idx - prev_idx))), NULL);
      } else {
        token->_next = create_token(
            _DATA_INTEGER,
            create_data(stoi(s.substr(prev_idx, curr_idx - prev_idx))), NULL);
      }

      token = token->_next;
    }

    else if (curr_chr == '"') {
      curr_idx += 1;
      prev_idx = curr_idx;
      for (; curr_idx < length; curr_idx++) {
        curr_chr = s[curr_idx];
        if (curr_chr == '"') {
          curr_idx += 1;
          break;
        }
      }
      token->_next = create_token(
          _DATA_STRING,
          create_data(s.substr(prev_idx, curr_idx - prev_idx - 1)), NULL);
      token = token->_next;
    }

    else if (is_alphabet(curr_chr)) {
      prev_idx = curr_idx;
      for (; curr_idx < length; curr_idx++) {
        curr_chr = s[curr_idx];
        if (!(is_alphabet(curr_chr) || curr_chr == '_' || is_number(curr_chr)))
          break;
      }
      _str = s.substr(prev_idx, curr_idx - prev_idx);

      if (_str == "if")
        token->_next = create_token(_IF, NULL, NULL);

      else if (_str == "else")
        token->_next = create_token(_ELSE, NULL, NULL);

      else if (_str == "elif")
        token->_next = create_token(_ELIF, NULL, NULL);

      else if (_str == "while")
        token->_next = create_token(_WHILE, NULL, NULL);

      else if (_str == "for")
        token->_next = create_token(_FOR, NULL, NULL);

      else if (_str == "in")
        token->_next = create_token(_IN, NULL, NULL);

      else if (_str == "return")
        token->_next = create_token(_RETURN, NULL, NULL);

      else if (_str == "lambda")
        token->_next = create_token(_LAMBDA, NULL, NULL);

      else if (_str == "and")
        token->_next = create_token(_AND, NULL, NULL);

      else if (_str == "or")
        token->_next = create_token(_OR, NULL, NULL);

      else if (_str == "not")
        token->_next = create_token(_NOT, NULL, NULL);

      else if (_str == "def")
        token->_next = create_token(_DEF, NULL, NULL);

      else if (_str == "class")
        token->_next = create_token(_CLASS, NULL, NULL);

      else if (_str == "True")
        token->_next = create_token(_TRUE, NULL, NULL);

      else if (_str == "False")
        token->_next = create_token(_FALSE, NULL, NULL);

      else if (_str == "import")
        token->_next = create_token(_IMPORT, NULL, NULL);

      else if (_str == "from")
        token->_next = create_token(_FROM, NULL, NULL);

      else if (_str == "pass")
        token->_next = create_token(_PASS, NULL, NULL);

      else if (_str == "break")
        token->_next = create_token(_BREAK, NULL, NULL);

      else if (_str == "continue")
        token->_next = create_token(_CONTINUE, NULL, NULL);

      else {
        token->_next = create_token(_VAR, create_data(_str), NULL);
      }

      token = token->_next;
    }

    else if (curr_chr == '=') {
      if ((curr_idx + 1) < length && s[curr_idx + 1] == '=') {
        curr_idx += 1;
        token->_next = create_token(_OP_EQUAL, NULL, NULL);
      } else if ((curr_idx + 1) < length && s[curr_idx + 1] == '<') {
        curr_idx += 1;
        token->_next = create_token(_OP_LTE, NULL, NULL);
      } else if ((curr_idx + 1) < length && s[curr_idx + 1] == '>') {
        curr_idx += 1;
        token->_next = create_token(_OP_GTE, NULL, NULL);
      } else
        token->_next = create_token(_ASSIGN, NULL, NULL);

      curr_idx += 1;
      token = token->_next;
    }

    else if (curr_chr == '<') {
      if ((curr_idx + 1) < length && s[curr_idx + 1] == '<') {
        curr_idx += 1;
        token->_next = create_token(_OP_SHIFT_LEFT, NULL, NULL);
      } else if ((curr_idx + 1) < length && s[curr_idx + 1] == '=') {
        curr_idx += 1;
        token->_next = create_token(_OP_LTE, NULL, NULL);
      } else
        token->_next = create_token(_OP_LT, NULL, NULL);

      curr_idx += 1;
      token = token->_next;
    }

    else if (curr_chr == '>') {
      if ((curr_idx + 1) < length && s[curr_idx + 1] == '>') {
        curr_idx += 1;
        token->_next = create_token(_OP_SHIFT_RIGHT, NULL, NULL);
      } else if ((curr_idx + 1) < length && s[curr_idx + 1] == '=') {
        curr_idx += 1;
        token->_next = create_token(_OP_GTE, NULL, NULL);
      } else
        token->_next = create_token(_OP_GT, NULL, NULL);

      curr_idx += 1;
      token = token->_next;
    }

    else if (curr_chr == '*') {
      if ((curr_idx + 1) < length && s[curr_idx + 1] == '*') {
        curr_idx += 1;
        token->_next = create_token(_OP_POW, NULL, NULL);
      } else
        token->_next = create_token(_OP_MUL, NULL, NULL);

      curr_idx += 1;
      token = token->_next;
    }

    else if (curr_chr == '\n') {
      token->_next = create_token(_ENDL, NULL, NULL);
      token = token->_next;
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

  token->_next = create_token(_ENDL, NULL, NULL);
  token = token->_next;
  token->_next = create_token(_END, NULL, NULL);
  return output;
}

void print_tokens(TOKEN *tokens) {
  for (TOKEN *token = tokens; token != NULL; token = token->_next) {
    cout << "token type: " << token_map[token->_type] << endl;
    if (token->_type == _DATA_INTEGER)
      cout << "data: " << token->_data->_integer << endl;
    else if (token->_type == _DATA_REAL)
      cout << "data: " << token->_data->_real << endl;
    else if (token->_type == _DATA_STRING)
      cout << "data: " << token->_data->_string << endl;
    else if (token->_type == _VAR)
      cout << "data: " << token->_data->_string << endl;
  }
}
