#include "multifile.hpp"

int main() {
  foo* f = make_foo();
  foo* r = make_bar();
  foo* z = make_baz();

  std::cout << f1(*f) << std::endl; // prints 5
  std::cout << f1(*r) << std::endl; // prints 42
  std::cout << f1(*z) << std::endl; // prints 10
  
  delete f;
  delete r;
  delete z;
  
  return 0;
}
