//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[pr_prefetch
/*`
  [pr_decl]
  [pr_functor]
  [pr_early]
 */
//]

#include "../classes.hpp"

using namespace rtti;

namespace {

//[pr_decl
/*`
  In the former example, the `DECLARE_MMETHOD` provides only the function object.

  Cases happen when you also need to access the function object's type.
  In those cases, the alternate forms `DECLARATION_MMETHOD`
  and `IMPLEMENTATION_MMETHOD`
  allow to define a function object type.
 */
using tags::_v;
DECLARATION_MMETHOD(prefetch_t, int, (_v<foo&>));

IMPLEMENTATION_MMETHOD(prefetch_t, int, (foo& a)) { return a.f(); }
IMPLEMENTATION_MMETHOD(prefetch_t, int, (bar& a)) { return a.g(); }
IMPLEMENTATION_MMETHOD(prefetch_t, int, (baz& a)) { return 2 * a.f(); }
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_prefetch) {
  //[pr_functor
  /*`
    In order to use the defined __multimethod__,
    you just have to create an object of that type.
   */
  lap l;
  prefetch_t prefetch;

  BOOST_CHECK_EQUAL( prefetch(l), 42 );
  //]

  //[pr_early
  /*`
    Using the function object, you can perform early dispatch
    with the `fetch` method.
    This method computed the multiple dispatch and returns a function pointer.
    The pointed function will perform the necessary static downcasts,
    and call the right implementation.
    The library guarantees that the return type of `fetch` is a function pointer,
    with the signature declared in `DECLARATION_MMETHOD`.
   */
  typedef prefetch_t::function_type func_t;   // function pointer type
  func_t fp = prefetch.fetch(l);              // call dispatch mechanism

  BOOST_STATIC_ASSERT( boost::is_same<func_t, int(*)(foo&)>::value );
  BOOST_CHECK_EQUAL( fp(l), 42 );             // downcast l and call second implementation
  //]
}
