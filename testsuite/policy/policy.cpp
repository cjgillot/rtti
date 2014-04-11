//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"
#include "boost/mmethod/policy/noreturn_policy.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
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
static bool called_ambiguous = false;

struct check_policy
: public boost::mmethod::ambiguity::noreturn_policy<check_policy>
{
  static void ambiguity_handler(std::size_t n, rtti_type const* types) {
    BOOST_CHECK_EQUAL(n, 2);
    BOOST_CHECK_EQUAL(types[0], static_id<foo2>());
    BOOST_CHECK_EQUAL(types[1], static_id<bar2>());

    BOOST_CHECK_EQUAL(found_ambiguous, false);
    found_ambiguous = true;
  }

  static void bad_dispatch() {
    called_ambiguous = true;
    throw boost::mmethod::bad_dispatch();
  }
};

using tags::_v;
BOOST_MMETHOD_DECLARE_POLICY(f1, int, (_v<foo1&>, _v<bar1&>), check_policy);

BOOST_MMETHOD_IMPLEMENT(f1, int, (foo1&, bar1&)) { return 0; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo2&, bar1&)) { return 13; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo1&, bar2&)) { return 8; }

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

  try {
    (void) f1(b, y);
  }
  catch(boost::mmethod::bad_dispatch&) {}

  BOOST_CHECK_EQUAL( called_ambiguous, true );
}
