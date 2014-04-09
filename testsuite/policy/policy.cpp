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

using rtti::mmethod::ambiguity::ambiguity_handler_t;

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

struct bar1
: base_rtti<bar1> {
public:
  virtual ~bar1() {}
};

struct bar2
: bar1
, implement_rtti<bar2, vector<bar1> >
{};

static bool found_ambiguous = false;

struct check_policy {
  static void apply(std::size_t n, rtti_type const* types) {
    BOOST_CHECK_EQUAL(n, 2);
    BOOST_CHECK_EQUAL(types[0], rtti::static_id<foo2>());
    BOOST_CHECK_EQUAL(types[1], rtti::static_id<bar2>());

    BOOST_CHECK_EQUAL(found_ambiguous, false);
    found_ambiguous = true;
  }

  static ambiguity_handler_t get_ambiguity_handler()
  { return &apply; }
};

using tags::_v;
DECLARE_MMETHOD_POLICY(f1, int, (_v<foo1&>, _v<bar1&>), check_policy);

IMPLEMENT_MMETHOD(f1, int, (foo1&, bar1&)) { return 0; }
IMPLEMENT_MMETHOD(f1, int, (foo2&, bar1&)) { return 13; }
IMPLEMENT_MMETHOD(f1, int, (foo1&, bar2&)) { return 8; }

// foo2-bar2 is ambiguous : foo2-bar1 and foo1-bar2 are equally good matches

} // namespace <>

BOOST_AUTO_TEST_CASE(policy) {
  foo1 a; foo2 b;
  bar1 x; bar2 y;

  f1.generate();

  BOOST_CHECK_EQUAL(found_ambiguous, true);

  BOOST_CHECK_EQUAL( f1(a, x),  0  ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(a, y),  8  ); // (1-2 case)
  BOOST_CHECK_EQUAL( f1(b, x), 13  ); // (2-1 case)
//   BOOST_CHECK_EQUAL( f1(b, y), 13  ); // (2-2 case)
}
