//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "./classes.hpp"

using namespace rtti;
using boost::mpl::vector;

namespace {

using tags::_v;
DECLARE_MMETHOD(binary, int, (_v<foo&>, _v<foo&>));

IMPLEMENT_MMETHOD(binary, int, (foo&, foo&)) { return 0; }
IMPLEMENT_MMETHOD(binary, int, (bar&, bar&)) { return 13; }
IMPLEMENT_MMETHOD(binary, int, (baz&, foo&)) { return 8; }
IMPLEMENT_MMETHOD(binary, int, (foo&, bar&)) { return 42; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_binary) {
  foo a; bar b; baz c;
  foo x; bar y; baz z;

  BOOST_CHECK_EQUAL( binary(a, x), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(a, y), 42  ); // (1-2 case)
  BOOST_CHECK_EQUAL( binary(a, z), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(b, x), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(b, y), 13  ); // (2-2 case)
  BOOST_CHECK_EQUAL( binary(b, z), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(c, x), 8   ); // (3-1 case)
  // binary(c, y) is resolved ambiguous   // (3-2 case)
  BOOST_CHECK_EQUAL( binary(c, z), 8   ); // (3-1 case)

  // ambiguous call throws
  BOOST_CHECK_EXCEPTION( binary(c, y), bad_dispatch, & );
}
