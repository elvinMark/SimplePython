#ifndef SPY_INTEGER
#define SPY_INTEGER

#include <spy/core/spy_float.hpp>
#include <spy/core/spy_object.hpp>

class SpyInteger : public SpyObject {
public:
  int _data;

public:
  SpyInteger(int x);
  SpyInteger *__add__(SpyInteger *a);
  SpyInteger *__sub__(SpyInteger *a);
  SpyInteger *__mul__(SpyInteger *a);
  SpyInteger **__divmod__(SpyInteger *a);
  SpyFloat *__float__();
  SpyInteger *__neg__();
  SpyInteger *__mod__();
  SpyFloat *__truediv__(SpyInteger *a);
};

#endif
