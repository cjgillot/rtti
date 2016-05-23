//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

using namespace rtti;

namespace {

using tags::_v;
DECLARE_MMETHOD(ambi, int, (_v<foo&>, _v<foo&>));

IMPLEMENT_MMETHOD(ambi, int, (foo&, foo&)) { return 0; }
IMPLEMENT_MMETHOD(ambi, int, (bar&, bar&)) { return 2; }
IMPLEMENT_MMETHOD(ambi, int, (lap&, lap&)) { return 4; }

IMPLEMENT_MMETHOD(ambi, int, (bar&, foo&)) { return 1; }
IMPLEMENT_MMETHOD(ambi, int, (foo&, bar&)) { return 1; }

int ref(foo&, foo&) { return 0; }
int ref(bar&, bar&) { return 2; }
int ref(lap&, lap&) { return 4; }
int ref(bar&, foo&) { return 1; }
int ref(foo&, bar&) { return 1; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_ambi) {
  foo f; bar r; lap l;

  BOOST_CHECK_EQUAL( ambi(f,f), ref(f,f) );
  BOOST_CHECK_EQUAL( ambi(r,f), ref(r,f) );
  BOOST_CHECK_EQUAL( ambi(l,f), ref(l,f) );

  BOOST_CHECK_EQUAL( ambi(f,r), ref(f,r) );
  BOOST_CHECK_EQUAL( ambi(r,r), ref(r,r) );
  BOOST_CHECK_EQUAL( ambi(l,r), ref(l,r) );

  BOOST_CHECK_EQUAL( ambi(f,l), ref(f,l) );
  BOOST_CHECK_EQUAL( ambi(r,l), ref(r,l) );
  BOOST_CHECK_EQUAL( ambi(l,l), ref(l,l) );
}
