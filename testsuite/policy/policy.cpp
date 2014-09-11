//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[po_policy
/*`
  [po_check_policy]
 */
//]

#include "./classes.hpp"

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"
#include "boost/mmethod/policy/noreturn_policy.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;

namespace {

//[po_check_policy
/*`
  The previous /policy/ class has one disadvantage :
  it cannot be reused for __multimethods__ with different signatures.

  In this example, we want to write a policy
  /bad dispatch/ that ignores its arguments and
  does not return.

  To do so, we will throw the following exception.
 */
struct check_exception
: public std::runtime_error {
  check_exception()
  : std::runtime_error("Ambiguous call !")
  {}
};

/*`
  We can now define our policy class.
  The `noreturn_policy` is a utility class
  defining the needed overloads to use
  the policy with any arity and any return type.
 */
struct check_policy
: boost::mmethod::ambiguity::noreturn_policy<check_policy>
{
  // optional ambiguity_handler()

  static void bad_dispatch() {
    boost::throw_exception( check_exception() );
  }
};

/*`
  Then, `check_policy` is used the same way than
  any other policy class.
 */
//]

using tags::_v;
BOOST_MMETHOD_DECLARE_POLICY(f1, int, (_v<foo1&>, _v<bar1&>), check_policy);

BOOST_MMETHOD_IMPLEMENT(f1, int, (foo1&, bar1&)) { return 0; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo2&, bar1&)) { return 13; }
BOOST_MMETHOD_IMPLEMENT(f1, int, (foo1&, bar2&)) { return 8; }

// foo2-bar2 is ambiguous : foo2-bar1 and foo1-bar2 are equally good matches

} // namespace <>

BOOST_AUTO_TEST_CASE(test_policy) {
  foo1 a; foo2 b;
  bar1 x; bar2 y;

  BOOST_CHECK_EQUAL( f1(a, x),  0  ); // (1-1 case)
  BOOST_CHECK_EQUAL( f1(a, y),  8  ); // (1-2 case)
  BOOST_CHECK_EQUAL( f1(b, x), 13  ); // (2-1 case)

  try {
    (void) f1(b, y);
  }
  catch(check_exception&) {}
}
