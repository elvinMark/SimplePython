#include <spy/utils/error.hpp>

void assert_error(int code_err) {
  if (code_err == ERR_INDENTATION)
    cout << "ERROR: Indentation error" << endl;
  else if (code_err == ERR_NO_TOKENS)
    cout << "ERROR: No tokens availables for the generation of the ast" << endl;
  else if (code_err == ERR_EXPECT_IDENTIFIER)
    cout << "ERROR: An IDENTIFIER was expected" << endl;
  else if (code_err == ERR_EXPECT_CONSTANT)
    cout << "ERROR: A CONSTANT was expected" << endl;
  else if (code_err == ERR_WRONG_TOKEN)
    cout << "ERROR: Wrong token encountered" << endl;
  else if (code_err == ERR_WRONG_AST)
    cout << "ERROR: Wrong ast encountered" << endl;
  else if (code_err == ERR_WRONG_CASTING)
    cout << "ERROR: Wrong casting was performed" << endl;
  else if (code_err == ERR_NOT_IMPLEMENTED)
    cout << "ERROR: Not implemented" << endl;
  else
    cout << "ERROR: Unknown" << endl;

  exit(-1);
}
