//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

#include <boost/test/unit_test.hpp>

using namespace rtti;

/*!\example dead.cpp
 *
 * Test whether do_fetch_pole can handle a dead branch.
 */

namespace {

using tags::_v;
DECLARE_MMETHOD(dead, int, (_v<foo&>));

IMPLEMENT_MMETHOD(dead, int, (bar&)) { return 5; }

} // namespace

BOOST_AUTO_TEST_CASE(test_dead) {
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( dead(r), 5  );
  BOOST_CHECK_EQUAL( dead(l), 5  );

  BOOST_CHECK_THROW( dead(f), bad_dispatch );
  BOOST_CHECK_THROW( dead(z), bad_dispatch );
}
