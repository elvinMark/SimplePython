#include <spy/core/object.hpp>

int is_integer(_object *_o1) { return _o1->_type == INTEGER_OBJECT; }
int is_real(_object *_o1) { return _o1->_type == REAL_OBJECT; }
int is_string(_object *_o1) { return _o1->_type == STRING_OBJECT; }
int is_number(_object *_o1) { return is_integer(_o1) || is_real(_o1); }
int is_list(_object *o1) { return o1->_type == LIST_OBJECT; }

int are_integer(_object *_o1, _object *_o2) {
  return is_integer(_o1) && is_integer(_o2);
}
int are_real(_object *_o1, _object *_o2) {
  return !are_integer(_o1, _o2) && are_number(_o1, _o2);
}
int are_string(_object *_o1, _object *_o2) {
  return is_string(_o1) && is_string(_o2);
}
int are_number(_object *_o1, _object *_o2) {
  return is_number(_o1) && is_number(_o2);
}

int are_list(_object *_o1, _object *_o2) {
  return is_list(_o1) && is_list(_o2);
}

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
  return *((string *)_o1->_data);
}

_list_object *get_list(_object *_o1) {
  if (_o1->_type != LIST_OBJECT)
    assert_error(ERR_WRONG_CASTING);
  return (_list_object *)_o1->_data;
}

_object *create_object(void *_data, int _type) {
  _object *_o = (_object *)malloc(sizeof(_object));
  _o->_data = _data;
  _o->_type = _type;
  return _o;
}

_object *create_integer_object(int d) {
  int *_d = (int *)malloc(sizeof(int));
  *_d = d;
  return create_object(_d, INTEGER_OBJECT);
}

_object *create_real_object(float f) {
  float *_f = (float *)malloc(sizeof(float));
  *_f = f;
  return create_object(&f, REAL_OBJECT);
}

_object *create_string_object(string s) {
  return create_object(new string(s), STRING_OBJECT);
}

_object *create_list(_list_object *l) { return create_object(l, LIST_OBJECT); }

_object *add_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) + get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    float f = num1 + num2;
    return create_real_object(f);
  } else if (are_string(_o1, _o2)) {
    return create_string_object(get_string(_o1) + get_string(_o2));
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *sub_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) - get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    float f = num1 - num2;
    return create_real_object(f);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *mult_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) * get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    float f = num1 * num2;
    return create_real_object(f);
  } else if (is_string(_o1) && is_integer(_o2)) {
    string s1 = get_string(_o1);
    string s2 = "";
    int d = get_integer(_o2);
    for (; d > 0; d--)
      s2 = s2 + s1;
    return create_string_object(s2);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *div_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) / get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    float f = num1 / num2;
    return create_real_object(f);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *pow_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = pow(get_integer(_o1), get_integer(_o2));
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    float f = powf(num1, num2);
    return create_real_object(f);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *mod_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) % get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    float f = 0; // num1 % num2;
    return create_real_object(f);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *lt_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) < get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    int d = num1 < num2;
    return create_integer_object(d);
  } else if (are_string(_o1, _o2)) {
    int d = get_string(_o1).length() < get_string(_o2).length();
    return create_integer_object(d);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *lte_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) <= get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    int d = num1 <= num2;
    return create_integer_object(d);
  } else if (are_string(_o1, _o2)) {
    int d = get_string(_o1).length() <= get_string(_o2).length();
    return create_integer_object(d);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *gt_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) > get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    int d = num1 > num2;
    return create_integer_object(d);
  } else if (are_string(_o1, _o2)) {
    int d = get_string(_o1).length() > get_string(_o2).length();
    return create_integer_object(d);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *gte_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) >= get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    int d = num1 >= num2;
    return create_integer_object(d);
  } else if (are_string(_o1, _o2)) {
    int d = get_string(_o1).length() >= get_string(_o2).length();
    return create_integer_object(d);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}

_object *eq_object(_object *_o1, _object *_o2) {
  if (are_integer(_o1, _o2)) {
    int d = get_integer(_o1) == get_integer(_o2);
    return create_integer_object(d);
  } else if (are_real(_o1, _o2)) {
    float num1 =
        _o1->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o1) : get_real(_o1);
    float num2 =
        _o2->_type == INTEGER_OBJECT ? 1.0 * get_integer(_o2) : get_real(_o2);
    int d = num1 == num2;
    return create_integer_object(d);
  } else if (are_string(_o1, _o2)) {
    int d = get_string(_o1).length() == get_string(_o2).length();
    return create_integer_object(d);
  } else {
    assert_error(ERR_NOT_IMPLEMENTED);
    return NULL;
  }
}
_object *in_object(_object *_o1, _object *_o2) {
  assert_error(ERR_NOT_IMPLEMENTED);
  return NULL;
}
