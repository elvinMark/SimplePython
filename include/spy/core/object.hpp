#ifndef SPY_OBJECT
#define SPY_OBJECT

#include <iostream>
#include <math.h>
#include <spy/utils/constants.hpp>
#include <spy/utils/error.hpp>
#include <string>

using namespace std;

typedef struct _object {
  void *_data;
  int _type;
} _object;

enum TYPE_DATA {
  INTEGER_OBJECT,
  REAL_OBJECT,
  STRING_OBJECT,
  LIST_OBJECT,
  DICT_OBJECT
};

int is_integer(_object *o1);
int is_real(_object *o1);
int is_string(_object *o1);
int is_number(_object *o1);

int are_integer(_object *_o1, _object *_o2);
int are_real(_object *_o1, _object *_o2);
int are_string(_object *_o1, _object *_o2);
int are_number(_object *_o1, _object *_o2);

int get_integer(_object *_o1);
float get_real(_object *_o1);
string get_string(_object *_o1);

_object *create_object(void *_data, int _type);
_object *create_integer_object(int d);
_object *create_real_object(float f);
_object *create_string_object(string s);

_object *add_object(_object *_o1, _object *_o2);
_object *sub_object(_object *_o1, _object *_o2);
_object *mult_object(_object *_o1, _object *_o2);
_object *div_object(_object *_o1, _object *_o2);
_object *pow_object(_object *_o1, _object *_o2);
_object *mod_object(_object *_o1, _object *_o2);
_object *lt_object(_object *_o1, _object *_o2);
_object *lte_object(_object *_o1, _object *_o2);
_object *gt_object(_object *_o1, _object *_o2);
_object *gte_object(_object *_o1, _object *_o2);
_object *eq_object(_object *_o1, _object *_o2);
_object *in_object(_object *_o1, _object *_o2);

#endif
