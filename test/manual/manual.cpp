//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

using namespace rtti;

namespace {

using tags::_v;
DECLARATION_MMETHOD(manual_t, int, (_v<foo const&>));

int impl1(foo const&) { return 1; }
int impl2(foo const&) { return 2; }
int impl3(foo const&) { return 3; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_manual) {
  manual_t manual;

  manual.insert<vector<foo&> >(&impl1);
  manual.insert<vector<bar&> >(&impl2);
  manual.insert<vector<baz&> >(&impl3);

  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( manual(f), 1 );
  BOOST_CHECK_EQUAL( manual(r), 2 );
  BOOST_CHECK_EQUAL( manual(z), 3 );
  BOOST_CHECK_EQUAL( manual(l), 2 ); // lap is-a bar
}
