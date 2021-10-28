#ifndef SPY_PARSER
#define SPY_PARSER

#include <cstring>
#include <fstream>
#include <iostream>
#include <spy/generator/ast.hpp>
#include <spy/generator/token.hpp>
#include <spy/utils/constants.hpp>
#include <spy/utils/error.hpp>
#include <spy/utils/misc.hpp>
#include <string>

using namespace std;

string code_from_ast(AST *_ast);
string code_from_tokens(TOKEN *tokens);
string code_from_string(string code);
string code_from_file(string path_to_file);

string generate_header();

#endif
