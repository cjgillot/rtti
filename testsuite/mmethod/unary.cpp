//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[un_unary
/*`
  [import ./classes.hpp]
  [un_hier]
  [un_decl]
  [un_impl]
  [un_use]
 */
//]

#include "./classes.hpp"

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;

namespace {

//[un_decl
/*`
  The declaration of a __multimethod__ is done via
  the `DECLARE_MMETHOD` macro. This macro takes three arguments :
  * the __multimethod__ name
  * the return type
  * the parenthesized parameter type list

  There can be two kinds of parameters : the neutral parameters
  and the dispatched parameters.
  Neutral parameters are just passed as-is to the called implementation.
  Dispatched parameters are those on which the multiple dispatch is computed.
  In order to differentiate between these, dispatched parameters must be tagged
  using the `tags::_v<>` template.
 */
using tags::_v;

BOOST_MMETHOD_DECLARE(f1, int, (_v<foo const&>));
/*`
  Here, the sole argument is a dispatched parameter.
  
  The dependency on __rtti__ appears : all dispatched parameters
  must have been registered with __rtti__ prior to the declaration.
 */
//]

//[un_impl
/*`
  After a __multimethod__ declaration, the `IMPLEMENT_MMETHOD` macro
  can be used to provide actual code for our __multimethod__.
  The parameters are like `DECLARE_MMETHOD` :
  * the __multimethod__ name
  * the return type
  * the parameter list
  
  Note that unlike the `DECLARE_MMETHOD` macro,
  no tagging is required on the parameters.
 */
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo const& a)) { return a.f(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (bar const& a)) { return a.g(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (baz const& a)) { return 2 * a.f(); }
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_unary) {
  //[un_use
  /*`
    Our __multimethod__ can now be used as a function object,
    all the dispatch is done transparently for the user.
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( f1(f),  5 );
  BOOST_CHECK_EQUAL( f1(r), 42 );
  BOOST_CHECK_EQUAL( f1(z), 10 );
  BOOST_CHECK_EQUAL( f1(l), 42 ); // (lap is-a bar)
  //]
}
