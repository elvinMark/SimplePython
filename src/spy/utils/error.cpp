#include <spy/utils/error.hpp>

void assert_error(int code_err) {
  if (code_err == ERR_INDENTATION)
    cout << "ERROR: Indentation error" << endl;
  else if (code_err == ERR_NO_TOKENS)
    cout << "ERROR: No tokens availables for the generation of the ast" << endl;
  else
    cout << "ERROR: Unknown" << endl;

  exit(-1);
}
