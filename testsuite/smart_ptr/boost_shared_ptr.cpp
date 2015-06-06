//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[bsp_boost_shared_ptr
/*`
  In order to demonstrate the use of smart pointers
  with __mmethod__, here is a example of dispatch through __shared_ptr__.

  [bsp_include]
  [bsp_declare]
  [bsp_invoke]
 */
//]

#include "../classes.hpp"

#include <boost/test/unit_test.hpp>

using namespace rtti::mmethod;

//[bsp_include
/*`
  We first need to include the following files,
  which will register __shared_ptr__ as a smart pointer
  for use with __mmethod__.
 */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "mmethod/smart_ptr/boost_shared_ptr.hpp"
//]

namespace {

//[bsp_declare
/*`
  The __shared_ptr__ type can now be used as a dispatched
  parameter, without further alteration of the __multimethod__ syntax.
 */
using tags::_v;
DECLARE_MMETHOD(bsp, int, (_v<boost::shared_ptr<foo> const&>));

/*`
  The __shared_ptr__ type is also be used on the specialized function.
 */
IMPLEMENT_MMETHOD(bsp, int, (boost::shared_ptr<foo> const& a)) { return a->f(); }
IMPLEMENT_MMETHOD(bsp, int, (boost::shared_ptr<bar> const& a)) { return a->g(); }
IMPLEMENT_MMETHOD(bsp, int, (boost::shared_ptr<baz> const& a)) { return 2 * a->f(); }
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_boost_shared_ptr) {
  //[bsp_invoke
  /*`
    Invocation of the __multimethod__ is also transparent
    for the user.
   */
  typedef boost::shared_ptr<foo> pointer;
  pointer f = boost::make_shared<foo>();
  pointer r = boost::make_shared<bar>();
  pointer z = boost::make_shared<baz>();
  pointer l = boost::make_shared<lap>();

  BOOST_CHECK_EQUAL( bsp(f),  5 );
  BOOST_CHECK_EQUAL( bsp(r), 42 );
  BOOST_CHECK_EQUAL( bsp(z), 10 );
  BOOST_CHECK_EQUAL( bsp(l), 42 ); // (lap is-a bar)
  //]
}
