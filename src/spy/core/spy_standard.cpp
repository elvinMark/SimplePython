#include <spy/core/spy_standard.hpp>

void print_object(_object *_o) { cout << get_string(str_object(_o)) << endl; }

void print_objects(_object *_o, ...) {
  va_list vl;
  va_start(vl, _o);

  while (_o != NULL) {
    print_object(_o);
    _o = va_arg(vl, _object *);
  }

  va_end(vl);
}

_object *input_object() {
  string s;
  getline(cin, s);
  return create_string_object(s);
}

_object *str_object(_object *_o1) {
  string s;
  if (_o1->_type == INTEGER_OBJECT) {
    s = to_string(get_integer(_o1));
  } else if (_o1->_type == REAL_OBJECT) {
    s = to_string(get_real(_o1));
  } else if (_o1->_type == STRING_OBJECT) {
    return _o1;
  } else
    assert_error(ERR_NOT_IMPLEMENTED);
  return create_string_object(s);
}

_object *int_object(_object *_o1) {
  int d = 0;
  if (_o1->_type == STRING_OBJECT)
    d = stoi(get_string(_o1));
  else
    assert_error(ERR_NOT_IMPLEMENTED);
  return create_integer_object(d);
}

_object *flaot_object(_object *_o1) {
  float f = 0;
  if (_o1->_type == STRING_OBJECT)
    f = stof(get_string(_o1));
  else
    assert_error(ERR_NOT_IMPLEMENTED);
  return create_real_object(f);
}
