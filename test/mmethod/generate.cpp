//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

using namespace rtti;

namespace {

//[ge_generate
/*`
  Any __multimethod__ must initialize its dispatch table at load time.
  This is normally done automatically at first call,
  via the `generate()` member function.
  This call ensures the dispatch table
  has been correctly generated
  before attempting to resolve a call.
 */
using tags::_v;
DECLARE_MMETHOD(shortcut, int, (_v<foo const&>));

IMPLEMENT_MMETHOD(shortcut, int, (foo const& a)) { return a.f(); }
IMPLEMENT_MMETHOD(shortcut, int, (bar const& a)) { return a.g(); }
IMPLEMENT_MMETHOD(shortcut, int, (baz const& a)) { return 2 * a.f(); }
//` [ge_use]
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_generate) {
  //[ge_use
  /*`
    At each call, the library will check whether
    the dispatch table has been generated.
    This should be a well-predicted branch,
    but we provide a way to shortcut this verification:

    First we need to call the `shortcut.generate()`
    method to force the computation of the dispatch table.

    Then, the following calls can be made using the `fast_call` method.
    A `fast_fetch` method exists to retrieve the code pointer.
   */
  // Ensure the table is generated
  shortcut.generate();

  // Use the __multimethod__
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( shortcut.fast_call(f),  5 );
  BOOST_CHECK_EQUAL( shortcut.fast_call(r), 42 );
  BOOST_CHECK_EQUAL( shortcut.fast_call(z), 10 );
  BOOST_CHECK_EQUAL( shortcut.fast_call(l), 42 ); // (lap is-a bar)
  /*`
    [warning
      The functions `fast_call` and `fast_fetch` exist for performance.
      Therefore, there is no runtime check.
      If the table has not been generated,
      these will fail miserably,
      and most probably crash the program.
    ]
   */
  //]
}
