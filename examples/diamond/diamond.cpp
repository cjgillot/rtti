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

/*!\example diamond.cpp
 * 
 * This example demonstrate mmethods with multiple inheritance.
 * 
 * Four classes are used : \c foo, \c bar, \c baz, \c lap
 * Their ids are then output on stdout
 * 
 * \c foo is the base class
 * \c bar has a statically defined id \c bar_id
 * \c baz and \c lap respectively inherit from \c foo and \c bar
 */

struct foo1
: base_rtti<foo1> {
public:
  virtual ~foo1() {}
};

struct foo2
: base_rtti<foo2> {
public:
  virtual ~foo2() {}
};

struct bar
: virtual foo1
, implement_rtti<bar, vector<foo1> >
{};

struct baz
: virtual foo1, foo2
, implement_rtti<baz, vector<foo1, foo2> >
{};

struct lap
: bar, baz
, implement_rtti<lap, vector<bar, baz> >
{};

using tags::_v;
BOOST_MMETHOD_DECLARE(f1, int, (_v<foo1&>));

BOOST_MMETHOD_IMPLEMENT(f1, int, (foo1&a)) { return 5; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (bar& a)) { return 7; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (lap& a)) { return 15; }

int main() {
  foo1 f; bar r; baz z; lap l;

  std::cout << f1(f) << std::endl; // prints 5
  std::cout << f1(r) << std::endl; // prints 7
  std::cout << f1(z) << std::endl; // prints 5
  std::cout << f1(l) << std::endl; // prints 15
  
  return 0;
}
