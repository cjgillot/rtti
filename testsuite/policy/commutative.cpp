//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "./classes.hpp"

#include <boost/test/unit_test.hpp>

#include <iostream>

using namespace rtti;

namespace {

//[co_commutative_policy
struct commutative_policy {
  static bool ambiguity_handler(std::size_t, rtti_type*);
  static int bad_dispatch(foo&, foo&);
};

bool
commutative_policy::ambiguity_handler(std::size_t n, rtti_type* types) {
  BOOST_CHECK_EQUAL(n, 2u);

  if(types[0] != types[1]) {
    std::swap(types[0], types[1]);
    return true;
  }

  return false;
}
//]

//[po_commutative_decl
using tags::_v;
DECLARE_MMETHOD_POLICY(comm, int, (_v<foo&>, _v<foo&>), commutative_policy);

// no foo-foo case
IMPLEMENT_MMETHOD(comm, int, (bar&, foo&)) { return 13; }
IMPLEMENT_MMETHOD(comm, int, (bar&, bar&)) { return 21; }

// foo-foo is not defined
// foo-bar is not defined either, but the policy resolves it to bar-foo

/*`
  `bad_dispatch` will serve as a catch-all implementation,
  so it can naturally implement what we would have set as `foo`-`foo`.
 */
int commutative_policy::bad_dispatch(foo& a, foo& x) {
  return 0;
}
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_commutative) {
  foo a; bar b;
  foo x; bar y;

  //[po_commutative_use
  BOOST_CHECK_EQUAL( comm(a, x),  0 ); // (1-1 case)
  BOOST_CHECK_EQUAL( comm(a, y), 13 ); // (1-2 case)
  BOOST_CHECK_EQUAL( comm(b, x), 13 ); // (2-1 case)
  BOOST_CHECK_EQUAL( comm(b, y), 21 ); // (2-2 case)
  //]
}
