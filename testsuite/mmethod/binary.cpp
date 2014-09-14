//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

namespace {

//[bi_binary
/*`
  Multiple arity __multimethods__ work exactly
  the same way than unary __multimethods__.
  In order to resolve overloads, we will use same
  rules than for static compiler-resolved calls.
  More precisely an implementation `F1` is determined to be better than `F2` if:

  * implicit conversions for all arguments of `F1` are /not worse/ than the implicit conversions for all arguments of `F2`,
  * there is at least one argument of `F1 whose implicit conversion is /better/ than the corresponding implicit conversion for that argument of `F2`.

  As a consequence, this behaviour can result into ambiguities.
  When an ambiguous dispatch is encountered,
  an exception of type `bad_dispatch` will be thrown.
 */
using tags::_v;
BOOST_MMETHOD_DECLARE(binary, int, (_v<foo&>, _v<foo&>));

BOOST_MMETHOD_IMPLEMENT(binary, int, (foo&, foo&)) { return 0; }
BOOST_MMETHOD_IMPLEMENT(binary, int, (bar&, bar&)) { return 13; }
BOOST_MMETHOD_IMPLEMENT(binary, int, (baz&, foo&)) { return 8; }
BOOST_MMETHOD_IMPLEMENT(binary, int, (foo&, bar&)) { return 42; }
//` [bi_use]
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_binary) {
//[bi_use
  foo a; bar b; baz c;
  foo x; bar y; baz z;

  BOOST_CHECK_EQUAL( binary(a, x), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(a, y), 42  ); // (1-2 case)
  BOOST_CHECK_EQUAL( binary(a, z), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(b, x), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(b, y), 13  ); // (2-2 case)
  BOOST_CHECK_EQUAL( binary(b, z), 0   ); // (1-1 case)
  BOOST_CHECK_EQUAL( binary(c, x), 8   ); // (3-1 case)
  // binary(c, y) is resolved ambiguous   // (3-2 case)
  BOOST_CHECK_EQUAL( binary(c, z), 8   ); // (3-1 case)

  // ambiguous call throws
  BOOST_CHECK_EXCEPTION( binary(c, y), bad_dispatch, & );
//]
}
