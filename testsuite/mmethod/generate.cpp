//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "./classes.hpp"

using namespace rtti;

namespace {

using tags::_v;
DECLARE_MMETHOD(generate, int, (_v<foo const&>));

IMPLEMENT_MMETHOD(generate, int, (foo const& a)) { return a.f(); }
IMPLEMENT_MMETHOD(generate, int, (bar const& a)) { return a.g(); }
IMPLEMENT_MMETHOD(generate, int, (baz const& a)) { return 2 * a.f(); }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_generate) {
  //[ge_generate
  /*`
    The call to the __multimethod__ `f1` automatically calls
    `f1.generate()`. This call ensures the dispatch table
    has been correctly generated.

    __mmethod__ provides a way to shortcut this verification.
    If `f1.generate()` has been called at a point in code :
   */
  generate.generate();
  /*`
    The following __multimethods__ calls can be made using
    the `fast_call`/`fast_fetch` pair :
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( generate.fast_call(f),  5 );
  BOOST_CHECK_EQUAL( generate.fast_call(r), 42 );
  BOOST_CHECK_EQUAL( generate.fast_call(z), 10 );
  BOOST_CHECK_EQUAL( generate.fast_call(l), 42 ); // (lap is-a bar)
  //]
}
