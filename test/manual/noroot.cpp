//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

using namespace rtti;

namespace {

using tags::_v;
DECLARATION_MMETHOD(man_noroot_t, int, (_v<foo const&>, _v<foo const&>));

int impl1(foo const&, foo const&) { return 1; }
int impl2(foo const&, foo const&) { return 2; }
int impl3(foo const&, foo const&) { return 3; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_man_noroot) {
  man_noroot_t man_noroot;

  man_noroot.insert<vector<foo const&, bar const&> >(&impl1);
  man_noroot.insert<vector<bar const&, foo const&> >(&impl2);
  man_noroot.insert<vector<bar const&, bar const&> >(&impl3);

  foo f; bar b;
  BOOST_CHECK_THROW( man_noroot(f,f), bad_dispatch );
  BOOST_CHECK_EQUAL( man_noroot(f,b), 1 );
  BOOST_CHECK_EQUAL( man_noroot(b,f), 2 );
  BOOST_CHECK_EQUAL( man_noroot(b,b), 3 );
}
