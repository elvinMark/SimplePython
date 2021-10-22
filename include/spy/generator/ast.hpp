#ifndef SPY_AST
#define SPY_AST

class AST {
public:
  AST *child_nodes;

public:
  AST();
  void add_child(AST *child_node);
};

#endif
