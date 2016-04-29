//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[un_unary
/*`
  [un_decl]
  [un_impl]
  [un_use]
 */
//]

#include "../classes.hpp"

using namespace rtti;

namespace {

//[un_decl
/*`
  The declaration of a __multimethod__ is done via
  the `DECLARE_MMETHOD` macro. This macro takes three arguments:

  * the __multimethod__ name
  * the return type
  * the parenthesized parameter type list

  There can be two kinds of parameters: the neutral parameters
  and the dispatched parameters.
  Neutral parameters are just passed as-is to the called implementation.
  Dispatched parameters are those on which the multiple dispatch is computed.
  In order to differentiate between these, dispatched parameters must be tagged
  using the `tags::_v<>` template.

  Here, the only argument is a dispatched parameter.
 */
using tags::_v;
DECLARE_MMETHOD(unary, int, (_v<foo const&>));

/*`
  The dependency on __rtti__ appears at this point. All dispatched parameters
  must have been registered with __rtti__ prior to the declaration of
  the __multimethod__.
 */
//]

//[un_impl
/*`
  After a __multimethod__ declaration, the `IMPLEMENT_MMETHOD` macro
  can be used to provide actual code for our __multimethod__.
  The parameters are like `DECLARE_MMETHOD` :

  * the __multimethod__ name
  * the return type
  * the actual parameter list

  The actual parameters are the types expected by the implementation.
  This set of parameters serves two things:
  to declare tha arguments of the function,
  and to register the association between this set of
  parameter types and the implementation.

  Unlike the `DECLARE_MMETHOD` macro,
  no tagging is required on the parameters.
 */
IMPLEMENT_MMETHOD(unary, int, (foo const& a)) { return a.f(); }
IMPLEMENT_MMETHOD(unary, int, (bar const& a)) { return a.g(); }
IMPLEMENT_MMETHOD(unary, int, (baz const& a)) { return 2 * a.f(); }
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_unary) {
  //[un_use
  /*`
    Our __multimethod__ can now be used as a function object,
    all the dispatch is done transparently for the user.
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( unary(f),  5 );
  BOOST_CHECK_EQUAL( unary(r), 42 );
  BOOST_CHECK_EQUAL( unary(z), 10 );
  BOOST_CHECK_EQUAL( unary(l), 42 ); // (lap is-a bar)
  //]
}
