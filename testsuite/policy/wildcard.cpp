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
  It has two static member functions:

  * `ambiguity_handler` (optional) modelling the /ambiguity handler/,
  * `bad_dispatch` modelling the /bad dispatch/ function.
 */
struct wildcard_policy {
  static void ambiguity_handler(std::size_t, rtti_type const*);
  static int bad_dispatch(foo&, foo&);
};

/*`
  `ambiguity_handler` is responsible for the handling of
  ambiguities at table generation time.
  It is called whenever the table generation algorithms
  runs into an ambiguous case.
  The passed parameters are:

  * the arity of the __multimethod__,
  * a C array of __rtti__ type ids.
 */
void
wildcard_policy::ambiguity_handler(std::size_t n, rtti_type const* types) {
  BOOST_CHECK_EQUAL(n, 2u);
  BOOST_CHECK_EQUAL(types[0], static_id<bar>());
  BOOST_CHECK_EQUAL(types[1], static_id<bar>());

  ++found_ambiguous;
  //=std::cout << "Ambiguity found !" << std::endl;
}

/*`
  `bad_dispatch` will be called whenever the user
  tries to invoke an ambiguous overload.
  Its job is to provide a default behaviour,
  when a specialized overload cannot be found.
  It is called with the arguments passed to the __multimethod__.
 */
int
wildcard_policy::bad_dispatch(foo& arg1, foo& arg2) {
  ++called_ambiguous;
  /*<-*/(void)arg1; (void)arg2;/*->*/
  //=std::cout << "Bad dispatch called with arguments: "
  //=          << &arg1 << " and " << &arg2 << std::endl;

  return 42;
}
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
DECLARE_MMETHOD_POLICY(f1, int, (_v<foo&>, _v<foo&>), wildcard_policy);

/*`
  Then, the implementation of the __multimethod__ need
  not care about the policy.
  The policy is perfectly transparent to the user
  providing the overloads.
 */
IMPLEMENT_MMETHOD(f1, int, (foo&, foo&)) { return 0; }
IMPLEMENT_MMETHOD(f1, int, (bar&, foo&)) { return 13; }
IMPLEMENT_MMETHOD(f1, int, (foo&, bar&)) { return 8; }

// bar-bar is ambiguous : bar-foo and foo-bar are equally good matches
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_wildcard) {
  foo a; bar b;
  foo x; bar y;

  //[po_wildcard_use
  /*`
    The runtime bahaviour is then the expected one :
   */
  // force generation
  f1.generate();

  // ambiguity_handler has been called
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
