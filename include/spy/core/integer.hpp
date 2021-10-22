#ifndef SPY_INTEGER
#define SPY_INTEGER

#include <spy/core/object.hpp>

class Integer : public Object {
public:
  Integer(int x);
  Integer *__add__(Integer *a);
  Integer *__neg__();
};

#endif
