//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

#include <iostream>

struct foo1
: base_rtti<foo1> {
public:
  virtual ~foo1() {}
};

struct foo2
: foo1
, implement_rtti<foo2, vector<foo1> >
{};

struct foo3
: foo1
, implement_rtti<foo3, vector<foo1> >
{};

struct bar1
: base_rtti<bar1> {
public:
  virtual ~bar1() {}
};

struct bar2
: bar1
, implement_rtti<bar2, vector<bar1> >
{};

struct bar3
: bar1
, implement_rtti<bar3, vector<bar1> >
{};

using tags::_v;
BOOST_MMETHOD_DECLARE(f1, int, (_v<foo1&>, _v<bar1&>));

BOOST_MMETHOD_IMPLEMENT(f1, int, (foo1& a, bar1& b)) { return 0; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo2& a, bar2& b)) { return 13; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo3& a, bar1& b)) { return 8; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo1& a, bar2& b)) { return 42; }

// foo3-bar2 is ambiguous : foo3-bar1 and foo1-bar2 are equally good matches
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo3& a, bar2& b)) { return 255; }

int main() {
  foo1 a; foo2 b; foo3 c;
  bar1 x; bar2 y; bar3 z;

  std::cout << f1(a, x) << std::endl; // prints 0    (1-1 case)
  std::cout << f1(a, y) << std::endl; // prints 42   (1-2 case)
  std::cout << f1(a, z) << std::endl; // prints 0    (1-1 case)
  std::cout << f1(b, x) << std::endl; // prints 0    (1-1 case)
  std::cout << f1(b, y) << std::endl; // prints 13   (2-2 case)
  std::cout << f1(b, z) << std::endl; // prints 0    (1-1 case)
  std::cout << f1(c, x) << std::endl; // prints 8    (3-1 case)
  std::cout << f1(c, y) << std::endl; // prints 255  (3-2 case)
  std::cout << f1(c, z) << std::endl; // prints 8    (3-1 case)

  return 0;
}
