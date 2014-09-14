//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

using namespace rtti;

namespace {

//[ge_generate
/*`
  The call to the __multimethod__ `generate` automatically calls
  its `generate.generate()` member function.
  This call ensures the dispatch table
  has been correctly generated
  before attempting to resolve a call.
 */
using tags::_v;
DECLARE_MMETHOD(generate, int, (_v<foo const&>));

IMPLEMENT_MMETHOD(generate, int, (foo const& a)) { return a.f(); }
IMPLEMENT_MMETHOD(generate, int, (bar const& a)) { return a.g(); }
IMPLEMENT_MMETHOD(generate, int, (baz const& a)) { return 2 * a.f(); }
//` [ge_use]
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_generate) {
  //[ge_use
  /*`
    At each call, the library will check whether
    the dispatch table has been generated.
    This should be a well-predicted branch,
    but we provide a way to shortcut this verification :

    First we need to call the `generate.generate()`
    method to force the computation of the dispatch table.

    Then, the following calls can be made using the `fast_call` method.
    A `fast_fetch` method exists to retrieve the code pointer.
   */
  // Ensure the table is generated
  generate.generate();

  // Use the __multimethod__
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( generate.fast_call(f),  5 );
  BOOST_CHECK_EQUAL( generate.fast_call(r), 42 );
  BOOST_CHECK_EQUAL( generate.fast_call(z), 10 );
  BOOST_CHECK_EQUAL( generate.fast_call(l), 42 ); // (lap is-a bar)
  //]
}
