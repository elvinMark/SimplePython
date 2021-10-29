#ifndef SPY_CONSTANTS
#define SPY_CONSTANTS

#define SPACES_INDENTATION 4

enum ERR_TYPE {
  ERR_INDENTATION,
  ERR_NO_TOKENS,
  ERR_EXPECT_IDENTIFIER,
  ERR_EXPECT_CONSTANT,
  ERR_WRONG_TOKEN,
  ERR_WRONG_AST
};

#endif
