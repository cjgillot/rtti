//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[po_wildcard
/*`
  [po_wildcard_policy]
  [po_wildcard_decl]
  [po_wildcard_use]
 */
//]

#include "./classes.hpp"

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"
#include "mmethod/policy/noreturn_policy.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace {

//[po_wildcard_policy
/*`
  In this example, we want to be able
  to flag any found ambiguity, both
  at generation time and at call time.
  Furthermore, we have a default implementation
  of the __multimethod__ in the ambiguous case.

  To remember the actual state,
  we will use the following global flags :
 */
int found_ambiguous  = 0;
int called_ambiguous = 0;

/*`
  We can now define our policy class.
  It has two static member functions :
  * `ambiguity_handler` (optional) modelling the /ambiguity handler/.
    It takes as parameter a C array of __rtti__ type ids.
  * `bad_dispatch` modelling the /bad dispatch/ function,
    taking the __multimethod__'s original arguments.
 */
struct wildcard_policy {
  // flag at generation time
  static void ambiguity_handler(std::size_t n, rtti_type const* types) {
    BOOST_CHECK_EQUAL(n, 2u);
    BOOST_CHECK_EQUAL(types[0], static_id<foo2>());
    BOOST_CHECK_EQUAL(types[1], static_id<bar2>());

    ++found_ambiguous;
  }

  // call time
  static int bad_dispatch(foo1&, bar1&) {
    ++called_ambiguous;

    // default implementation
    return 42;
  }
};
//]

//[po_wildcard_decl
/*`
  After a policy class has been defined,
  we can declare a __multimethod__ using it.
  We only have to replace `DECLARE_MMETHOD`
  by `DECLARE_MMETHOD_POLICY` and add the policy
  as fourth argument.
 */
using tags::_v;
DECLARE_MMETHOD_POLICY(f1, int, (_v<foo1&>, _v<bar1&>), wildcard_policy);

IMPLEMENT_MMETHOD(f1, int, (foo1&, bar1&)) { return 0; }
IMPLEMENT_MMETHOD(f1, int, (foo2&, bar1&)) { return 13; }
IMPLEMENT_MMETHOD(f1, int, (foo1&, bar2&)) { return 8; }

// foo2-bar2 is ambiguous : foo2-bar1 and foo1-bar2 are equally good matches
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_wildcard) {
  foo1 a; foo2 b;
  bar1 x; bar2 y;

  //[po_wildcard_use
  /*`
    The runtime bahaviour is then the expected one :
   */
  // force generation
  f1.generate();

  // `wildcard_policy::ambiguity_handler` has been called
  BOOST_CHECK_EQUAL( found_ambiguous, 1 );

  // regular calls
  BOOST_CHECK_EQUAL( f1(a, x),  0 ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(a, y),  8 ); // (1-2 case)
  BOOST_CHECK_EQUAL( f1(b, x), 13 ); // (2-1 case)
  BOOST_CHECK_EQUAL( called_ambiguous, 0 );

  // ambiguous call
  BOOST_CHECK_EQUAL( f1(b, y), 42 ); // (2-2 case : ambiguous)
  BOOST_CHECK_EQUAL( called_ambiguous, 1 );
  //]
}
