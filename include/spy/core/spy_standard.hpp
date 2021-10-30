#ifndef SPY_STANDARD
#define SPY_STANDARD

#include <iostream>
#include <spy/core/object.hpp>
#include <stdarg.h>
#include <string>

void print_object(_object *_o);
void print_objects(_object *_o, ...);
_object *input_object();
_object *str_object(_object *_o1);
_object *int_object(_object *_o1);
_object *flaot_object(_object *_o1);

#endif
