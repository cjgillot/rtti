//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[cov_covariant
/*`
  [cov_decl]
  [cov_impl]
  [cov_use]
 */
//]

#include "../classes.hpp"

using namespace rtti;

namespace {

//[cov_decl
/*`
  The return type os a __multimethod__ can also be a covariant return type.
 */
using tags::_v;
DECLARE_MMETHOD(covariant, foo const*, (_v<foo const&>));
//]

//[cov_impl
/*`
  The covariant return type, as for virtual functions,
  is defined by just changiing the return type for implementations.
 */
IMPLEMENT_MMETHOD(covariant, foo const*, (foo const& a)) { return &a; }
IMPLEMENT_MMETHOD(covariant, bar const*, (bar const& a)) { return &a; }
IMPLEMENT_MMETHOD(covariant, baz const*, (baz const& a)) { return &a; }
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_covariant) {
  //[cov_use
  /*`
    The __mtultimethod__ can then be used as any virtual
    function with covariant return type.
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( covariant(f), &f );
  BOOST_CHECK_EQUAL( covariant(r), &r );
  BOOST_CHECK_EQUAL( covariant(z), &z );
  BOOST_CHECK_EQUAL( covariant(l), &l ); // (lap is-a bar)
  //]
}
