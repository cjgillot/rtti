//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "./classes.hpp"

using namespace rtti;
using boost::mpl::vector;

namespace {

using tags::_v;
DECLARE_MMETHOD(mixed, int, (long, _v<foo const&>));

IMPLEMENT_MMETHOD(mixed, int, (long n, foo const& a)) { return n + a.f(); }
IMPLEMENT_MMETHOD(mixed, int, (long n, bar const& a)) { return n + a.g(); }
IMPLEMENT_MMETHOD(mixed, int, (long n, baz const& a)) { return n * a.f(); }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_mixed) {
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( mixed(1, f),  6 );
  BOOST_CHECK_EQUAL( mixed(2, r), 44 );
  BOOST_CHECK_EQUAL( mixed(3, z), 15 );
  BOOST_CHECK_EQUAL( mixed(9, l), 51 ); // (lap is-a bar)
}
