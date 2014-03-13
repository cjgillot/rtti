//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
