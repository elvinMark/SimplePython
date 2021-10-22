# SPY: Simple PYthon

This is a simple Python-to-C++ translator. Just a simple personal
project to practice and understand more a little bit about parsing and
ast.

# TODO
- [ ] Parser:
  - [x] Generate Tokens: Raw Code -> Sequence of Tokens ([def,test,(,if,x,<,....])
  - [ ] Generate AST: Sequence of Tokens -> AST (fun_statement, if_statement)
- [ ] Code Generator:
  - [ ] Sequence of Tokens -> C++ code
