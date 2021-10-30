#include <spy/core/object.hpp>

int asdfad(double x) { return 2; }

int main() {
  // int d1 = 2;
  // int d2 = 3;
  // float f1 = 4.5;
  // float f2 = 6.7;
  // string s1 = "Hello";
  // string s2 = "World";

  // _object *_o1 = create_object(&d1, INTEGER_OBJECT);
  // _object *_o2 = create_object(&d2, INTEGER_OBJECT);
  // _object *_o3 = create_object(&f1, REAL_OBJECT);
  // _object *_o4 = create_object(&f2, REAL_OBJECT);
  // _object *_o5 = create_object(&s1, STRING_OBJECT);
  // _object *_o6 = create_object(&s2, STRING_OBJECT);

  _object *_o1 = create_integer_object(2);
  _object *_o2 = create_integer_object(3);
  _object *_o3 = create_real_object(4.5);
  _object *_o4 = create_real_object(6.7);
  _object *_o5 = create_string_object("HELLO");
  _object *_o6 = create_string_object("WORLD");

  _object *o;

  o = mult_object(_o5, pow_object(_o1, _o2));
  cout << get_string(o) << endl;
  return 0;
}
