//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

/*!\example diamond.cpp
 *
 * Test whether do_fetch_pole can handle a dead branch.
 */

namespace {

struct root
: base_rtti<root, virtual_mixin_node> {
public:
  virtual ~root() {}
};

struct foo1
: virtual root
, implement_rtti<foo1, vector<root> >
{};

struct foo2
: virtual root
, implement_rtti<foo2, vector<root> >
{};

struct baz
: foo1, foo2
, implement_rtti<baz, vector<foo1, foo2> >
{};

using tags::_v;
BOOST_MMETHOD_DECLARE(f1, int, (_v<root&>));

BOOST_MMETHOD_IMPLEMENT(f1, int, (foo2&)) { return 5; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_dead) {
  root r; foo1 f; foo2 g; baz z;

  BOOST_CHECK_EQUAL( f1(g), 5  );
  BOOST_CHECK_EQUAL( f1(z), 5  );

  try {
    (void) f1(r);
    BOOST_CHECK(false);
  }
  catch(bad_dispatch&) {
    BOOST_CHECK(true);
  }

  try {
    (void) f1(f);
    BOOST_CHECK(false);
  }
  catch(bad_dispatch&) {
    BOOST_CHECK(true);
  }
}
