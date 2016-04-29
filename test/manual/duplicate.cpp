//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

using namespace rtti;

namespace {

using tags::_v;
DECLARATION_MMETHOD(man_dup_t, int, (_v<foo const&>));

int impl1(foo const&) { return 1; }
int impl2(foo const&) { return 2; }
int impl3(foo const&) { return 3; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_man_dup) {
  man_dup_t man_dup;

  man_dup.insert<vector<foo const&> >(&impl1);
  man_dup.insert<vector<bar const&> >(&impl2);
  man_dup.insert<vector<bar const&> >(&impl3);

  BOOST_CHECK_THROW( man_dup.generate(), std::runtime_error );
  BOOST_CHECK_EQUAL( impl1(foo()), 1 );
  BOOST_CHECK_EQUAL( impl2(foo()), 2 );
  BOOST_CHECK_EQUAL( impl3(foo()), 3 );
}
