#include <spy/generator/token.hpp>

DATA null_data;

char token_type_map[56][30] = {"_EMPTY",
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

TOKEN *create_token() { return create_token(_EMPTY, null_data, NULL); }

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
          (c <= 96 && c >= 91) || (c <= 126 && c >= 123));
}

void create_map_characters(char *token_map) {
  token_map['+'] = _OP_ADD;
  token_map['-'] = _OP_DIF;
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
