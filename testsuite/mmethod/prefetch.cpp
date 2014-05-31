//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[pr_prefetch
/*`
  In the former example, the `DECLARE_MMETHOD` provides only the function object.
  
  Cases happen when you also need to access the function object's type.
  In those cases, the alternate forms `DECLARATION_MMETHOD` and `IMPLEMENTATION_MMETHOD`
  allow you to define a function object type.
  [pr_decl]
  
  In order to use the defined __multimethod__,
  you just have to create an object of that type.
  [pr_functor]
  
  Using the function object, you can perform early dispatch with the `fetch` method.
  This method computed the multiple dispatch and returns a code pointer.
  The pointed code will only perform the necessary static downcasts,
  and call the meant implementation.
  [pr_early]
 */
//]

#include "./classes.hpp"

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;
using boost::mpl::vector;

namespace {

//[pr_decl
using tags::_v;
DECLARATION_MMETHOD(f1_t, int, (_v<foo&>));

IMPLEMENTATION_MMETHOD(f1_t, int, (foo& a)) { return a.f(); }
IMPLEMENTATION_MMETHOD(f1_t, int, (bar& a)) { return a.g(); }
IMPLEMENTATION_MMETHOD(f1_t, int, (baz& a)) { return 2 * a.f(); }
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(prefetch) {
  //[pr_functor
  lap l;
  f1_t f1;
  
  BOOST_CHECK_EQUAL( f1(l), 42 );
  //]

  //[pr_early
  typedef f1_t::function_type func_t;   // function pointer type
  func_t fp = f1.fetch(l);              // call dispatch mechanism

  BOOST_CHECK_EQUAL( fp(l), 42 );       // downcast `l` and call `l.bar::g()`
  //]
}
