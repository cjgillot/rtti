//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;
using boost::mpl::vector;

namespace {

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
DECLARE_MMETHOD(f1, int, (_v<foo1&>, _v<bar1&>));

IMPLEMENT_MMETHOD(f1, int, (foo1&, bar1&)) { return 0; }
IMPLEMENT_MMETHOD(f1, int, (foo2&, bar2&)) { return 13; }
IMPLEMENT_MMETHOD(f1, int, (foo3&, bar1&)) { return 8; }
IMPLEMENT_MMETHOD(f1, int, (foo1&, bar2&)) { return 42; }

// foo3-bar2 is ambiguous : foo3-bar1 and foo1-bar2 are equally good matches
IMPLEMENT_MMETHOD(f1, int, (foo3&, bar2&)) { return 255; }

} // namespace <>

BOOST_AUTO_TEST_CASE(binary) {
  foo1 a; foo2 b; foo3 c;
  bar1 x; bar2 y; bar3 z;

  BOOST_CHECK_EQUAL( f1(a, x), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(a, y), 42  ); // (1-2 case)
  BOOST_CHECK_EQUAL( f1(a, z), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(b, x), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(b, y), 13  ); // (2-2 case)
  BOOST_CHECK_EQUAL( f1(b, z), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(c, x), 8   ); // (3-1 case)
  BOOST_CHECK_EQUAL( f1(c, y), 255 ); // (3-2 case)
  BOOST_CHECK_EQUAL( f1(c, z), 8   ); // (3-1 case)
}
