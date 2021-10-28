# SPY: Simple PYthon

This is a simple Python-to-C++ translator. Just a simple personal
project to practice and understand more a little bit about parsing and
ast.

# Simple Python
I am going to use a very simple python-like programming language. The
following feature are going to be supported for this simple python:

- Basic types: int, float, string and array

```python
x=3
y=2.4
z="hello!"
w=[x,y,z]
```

- Basic Operators:

```python
# Mathematical Operators
z = x + y 
z = x - y 
z = x * y 
z = x / y 

# Boolean Operators
z = x or y
z = x and y
z = not x
z = x < y
z = x <= y
z = x >= y
z = x > y
z = x == y

z = x | y
z = x & y
z = x ^ y

```

- Conditionals 

```python
if CONDITION : 
	DO_STH
elif CONDITION: # optional
	DO_STH
else:           # optional
	DO_STH
```
- Loops

```python
# For loops
for i in ITERATOR_OR_ARR:
	DO_STH
	
# While loops
while CONDITION:
	DO_STH

```

- Functions

```python
def FUN(ARGS):
	DO_STH
	return STH

# Also lambda expressions
f = lambda ARGS: DO_STH

```

- Classes

```python
class CLASS(SUPERCLASS):
	def METHOD(ARGS):
		DO_STH
		return STH
```

- Import 

```python
import MODULE
from MODULE import SUBMODULE
```

# Grammar

- MODULE: STMTS

- STMTS: STMT +

- EXPRS: EXPR +

- IDENTIFIER

- CONSTANT

- STMT: 
  - ASSIGN (AUGASSIGN [x += 3] is not supported yet):
	- EXPR
	- EXPR
  - CLASSDEF:
	- IDENTIFIER
	- EXPRS
	- STMTS
  - FUNCTIONDEF:
	- IDENTIFIER
	- ARGUMENTS
	- STMTS
	- RETURN?
  - RETURN:
	- EXPR
  - IF:
	- EXPR
	- STMTS
	- ELSE
  - ELSE:
	- STMTS
  - FOR:
	- INITERATOR
	- STMTS
  - WHILE:
	- EXPR
	- STMTS
  - IMPORT:
	- IDENTIFIER
  - IMPORTFROM:
	- IDENTIFIER
	- IDENTIFIER
  - PASS
  - BREAK
  - CONTINUE
  - EXPR
	
- EXPR:
  - IDENTIFIER
  - CONSTANT
  - LIST
  - DICT
  - CALL
  - SUBSCRIPT
  - BOOLOP
  - BINOP
  - UNARYOP
  - CMPOP
  - IFEXP
  - LAMBDA

- INITERATOR:
  - IDENTIFIER
  - EXPR

- LIST:
  - EXPRS

- DICT:
  - EXPRS
  - EXPRS

- CALL:
  - IDENTIFIER
  - EXPRS

- SUBSCRIPT:
  - IDENTIFIER
  - SLICE
  
- SLICE:
  - EXPR
  - EXPR?
  - EXPR?

- IFEXP:
  - EXPR
  - EXPR
  - EXPR

- LAMBDA:
  - ARGUMENTS
  - EXPR

- BOOLOP:
  - AND:
	- EXPR
	- EXPR
  - OR:
  	- EXPR
	- EXPR

- BINOP:
  - ADD
  - SUB
  - MULT
  - DIV
  - MOD
  - POW
  - LShift
  - RShift
  - BitOr
  - BitAnd
  - BitXor

- UNARYOP:
  - NOT

- CMPOP:
  - EQ
  - LT
  - LTE
  - GT
  - GTE
  - IN

- ARGUMENTS:
  - IDENTIFIERS

# TODO
- [x] Parser:
  - [x] Generate Tokens: Raw Code -> Sequence of Tokens ([def,test,(,if,x,<,....])
  - [x] Generate AST: Sequence of Tokens -> AST (fun_statement, if_statement) [~Almost done~]
- [ ] Code Generator:
  - [ ] Sequence of Tokens -> C++ code
