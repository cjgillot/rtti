//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "rtti/rtti.hpp"

#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <iostream>
#include <boost/mpl/vector.hpp>

using namespace rtti;
using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
  
  int f() { return 5; }
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{
  int g() { return 42; }
};

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};

using tags::_v;
DECLARE_MMETHOD(f1, int, (_v<foo>&));

IMPLEMENT_MMETHOD(f1, int, (foo& a)) { return a.f(); }
IMPLEMENT_MMETHOD(f1, int, (bar& a)) { return a.g(); }
IMPLEMENT_MMETHOD(f1, int, (baz& a)) { return 2 * a.f(); }

int main() {
  foo f; bar r; baz z; lap l;

  std::cout << f1(f) << std::endl; // prints 5
  std::cout << f1(r) << std::endl; // prints 42
  std::cout << f1(z) << std::endl; // prints 10
  std::cout << f1(l) << std::endl; // prints 42 (lap is-a bar)
  
  return 0;
}
