#ifndef SPY_TOKEN
#define SPY_TOKEN

#include <iostream>

enum TOKEN_TYPE {
  _EMPTY,
  _NONE,
  _DATA_INTEGER,
  _DATA_REAL,
  _DATA_STRING,
  _CLASS,
  _DEF,
  _IF,
  _ELSE,
  _ELIF,
  _WHILE,
  _FOR,
  _IN,
  _RETURN,
  _LAMBDA,
  _IMPORT,
  _WITH,
  _AS,
  _TRY,
  _EXCEPT,
  _RAISE,
  _ASSERT,
  _PASS,
  _BREAK,
  _CONTINUE,
  _GLOBAL,
  _AND,
  _OR,
  _NOT,
  _VAR,
  _TRUE,
  _FALSE,
  _OPEN_CURLY_BRACKET,
  _CLOSE_CURLY_BRACKET,
  _OPEN_ROUND_BRACKET,
  _CLOSE_ROUND_BRACKET,
  _OPEN_SQUARE_BRACKET,
  _CLOSE_SQUARE_BRACKET,
  _COLON,
  _COMMA,
  _DOT,
  _QUOTE,
  _DOUBLE_QUOTE,
  _START,
  _END,
  _ASSIGN,
  _OP_ADD,
  _OP_DIF,
  _OP_MUL,
  _OP_DIV,
  _OP_POW,
  _OP_GT,
  _OP_LT,
  _OP_EQUAL,
  _OP_AND,
  _OP_OR,
  _OP_XOR,
  _OP_MOD,
  _OP_NOT,
  _OP_SHIFT_LEFT,
  _OP_SHIFT_RIGHT
};

union DATA {
  int _integer;
  double _real;
  char *_string;
};

extern DATA null_data;
extern char token_type_map[61][30];

typedef struct TOKEN {
  int _type;
  DATA _data;
  struct TOKEN *_next_token;
} TOKEN;

TOKEN *create_token();
TOKEN *create_token(int, DATA, TOKEN *);
int is_number(char c);
int is_alphabet(char c);
int is_symbol(char c);
void create_map_characters(char *map);

#endif
