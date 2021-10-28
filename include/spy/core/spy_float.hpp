#ifndef SPY_FLOAT
#define SPY_FLOAT

#include <spy/core/spy_object.hpp>

class SpyFloat : public SpyObject {
public:
  int _data;

public:
  SpyFloat(int x);
  SpyFloat *__add__(SpyFloat *a);
  SpyFloat *__sub__(SpyFloat *a);
  SpyFloat *__mul__(SpyFloat *a);
  SpyFloat **__divmod__(SpyFloat *a);
  SpyInteger *__int__();
  SpyFloat *__neg__();
  SpyFloat *__mod__();
  SpyFloat *__truediv__(SpyFloat *a);
};
#endif
