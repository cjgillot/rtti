//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

using namespace rtti;

namespace {

//[mi_mixed
/*`
  Some times, additional neutral parameters are required.
  These can be needed to pass some state around,
  or simply a formal parameter to the __multimethod__.
  Here is an example of mixing dispatched and neutral parameters.
 */
using tags::_v;
DECLARE_MMETHOD(mixed, int, (long, _v<foo const&>));

IMPLEMENT_MMETHOD(mixed, int, (long n, foo const& a)) { return n + a.f(); }
IMPLEMENT_MMETHOD(mixed, int, (long n, bar const& a)) { return n + a.g(); }
IMPLEMENT_MMETHOD(mixed, int, (long n, baz const& a)) { return n * a.f(); }
//` [mi_use]
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_mixed) {
  foo f; bar r; baz z; lap l;

  //[mi_use
  /*`
    Again the use is straightforward.
   */
  BOOST_CHECK_EQUAL( mixed(1, f),  6 );
  BOOST_CHECK_EQUAL( mixed(2, r), 44 );
  BOOST_CHECK_EQUAL( mixed(3, z), 15 );
  BOOST_CHECK_EQUAL( mixed(9, l), 51 ); // (lap is-a bar)
  //]
}
