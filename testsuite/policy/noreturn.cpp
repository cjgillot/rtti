//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[po_policy
/*`
  [po_check_policy]
 */
//]

#include "./classes.hpp"

#include "mmethod/policy/noreturn_policy.hpp"

#include <boost/test/unit_test.hpp>

using namespace rtti;

namespace {

//[po_check_policy
/*`
  The previous policy class has one disadvantage:
  its /bad dispatch/ function cannot used
  for __multimethods__ with different signatures.
  Although this is fine most of the time,
  it may be interessant to have more general
  "catch-all" policy classes.
  In this example, we want to write a policy class
  whose /bad dispatch/ function ignores its arguments and
  does not return.

  To do so, we will throw the following exception:
 */
struct check_exception
: public std::runtime_error
{
  check_exception()
  : std::runtime_error("Ambiguous call")
  {}
};

/*`
  We can now define our policy class.
  The `noreturn_policy` is a utility class
  defining the needed overloads to use
  the policy with any arity and any return type.
 */
struct check_policy
: private mmethod::ambiguity::default_policy
{
  using mmethod::ambiguity::default_policy::ambiguity_handler;

  static void bad_dispatch() {
    BOOST_THROW_EXCEPTION( check_exception() );
  }
};

/*`
  Then, `check_policy` is used the same way than
  any other policy class.
 */
//]

using tags::_v;
DECLARE_MMETHOD_POLICY(f1, int, (_v<foo&>, _v<foo&>), check_policy);

IMPLEMENT_MMETHOD(f1, int, (foo&, foo&)) { return 0; }
IMPLEMENT_MMETHOD(f1, int, (bar&, foo&)) { return 13; }
IMPLEMENT_MMETHOD(f1, int, (foo&, bar&)) { return 8; }

// bar-bar is ambiguous : bar-foo and foo-bar are equally good matches

} // namespace <>

BOOST_AUTO_TEST_CASE(test_policy) {
  foo a; bar b;
  foo x; bar y;

  BOOST_CHECK_EQUAL( f1(a, x),  0  ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(a, y),  8  ); // (1-2 case)
  BOOST_CHECK_EQUAL( f1(b, x), 13  ); // (2-1 case)

  BOOST_CHECK_THROW( f1(b, y), check_exception );
}
