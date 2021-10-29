#include <spy/core/object.hpp>

int is_integer(_object *_o1) { return _o1->_type == INTEGER_OBJECT; }
int is_real(_object *_o1) { return _o1->_type == REAL_OBJECT; }
int is_string(_object *_o1) { return _o1->_type == STRING_OBJECT; }
int is_number(_object *_o1) { return is_integer(_o1) || is_real(_o1); }
int is_string(_object *_o1) { return _o1->_type == STRING_OBJECT; }

int are_integer(_object *_o1, _object *_o2) {
  return is_integer(_o1) && is_integer(_o2);
}
int are_real(_object *_o1, _object *_o2) {
  return !are_integer(_o1, _o2) && !are_number(_o1, _o2);
}
int are_string(_object *_o1, _object *_o2) {
  return is_string(_o1) && is_string(_o2);
}
int are_number(_object *_o1) { return is_number(_o1) && is_number(_o2); }

int get_integer(_object *_o1) {
  if (_o1->_type != INTEGER_OBJECT)
    assert_error(ERR_WRONG_CASTING);
  return *(int *)_o1->_data;
}

float get_real(_object *_o1) {
  if (_o1->_type != REAL_OBJECT)
    assert_error(ERR_WRONG_CASTING);
  return *(float *)_o1->_data;
}

string get_string(_object *_o1) {
  if (_o1->_type != STRING_OBJECT)
    assert_error(ERR_WRONG_CASTING);
  return *(string *)_o1->_data;
}

_object *create_object(void *_data, int _type) {
  _object *_o = (_object *)malloc(sizeof(_object));
  _o->_data = _data;
  _o->_type = _type;
  return _o;
}

_object *add_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1->_data) + get_integer(_o2->_data);
    return create_object(&d, INTEGER_OBJECT);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    float f = num1 + num2;
    return create_object(&f, REAL_OBJECT);
  } else if (are_string(_o1, _o2)) {
    string s = get_string(_o1) + get_string(_o2);
    return create_object(&s, STRING_OBJECT);
  }
}
