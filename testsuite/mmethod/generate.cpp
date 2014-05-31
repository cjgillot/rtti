//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "./classes.hpp"

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace {

using tags::_v;
DECLARE_MMETHOD(f1, int, (_v<foo const&>));

IMPLEMENT_MMETHOD(f1, int, (foo const& a)) { return a.f(); }
IMPLEMENT_MMETHOD(f1, int, (bar const& a)) { return a.g(); }
IMPLEMENT_MMETHOD(f1, int, (baz const& a)) { return 2 * a.f(); }

} // namespace <>

BOOST_AUTO_TEST_CASE(generate) {
  //[ge_generate
  /*`
    The call to the __multimethod__ `f1` automatically calls
    `f1.generate()`. This call ensures the dispatch table
    has been correctly generated.

    __mmethod__ provides a way to shortcut this verification.
    If `f1.generate()` has been called at a point in code :
   */
  f1.generate();
  /*`
    The following __multimethods__ calls can be made using
    the `fast_call`/`fast_fetch` pair :
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( f1.fast_call(f),  5 );
  BOOST_CHECK_EQUAL( f1.fast_call(r), 42 );
  BOOST_CHECK_EQUAL( f1.fast_call(z), 10 );
  BOOST_CHECK_EQUAL( f1.fast_call(l), 42 ); // (lap is-a bar)
  //]
}
