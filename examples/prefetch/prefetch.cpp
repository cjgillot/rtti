//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <iostream>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
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
BOOST_MMETHOD_DECLARE(f1, int, (_v<foo&>));

BOOST_MMETHOD_IMPLEMENT(f1, int, (foo& a)) { return a.f(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (bar& a)) { return a.g(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (baz& a)) { return 2 * a.f(); }

int main() {
  lap l;

  typedef BOOST_MMETHOD_TAG(f1)::function_type func_t;
  func_t fp = f1.fetch(l);

  std::cout << fp(l) << std::endl; // prints 42

  return 0;
}
