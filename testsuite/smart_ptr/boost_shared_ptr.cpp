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
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

//[bsp_include
/*`
  We first need to include the following files,
  which will register __shared_ptr__ as a smart pointer
  for use with __mmethod__.
 */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/mmethod/smart_ptr/boost_shared_ptr.hpp>
//]

namespace {

//[bsp_declare
/*`
  The __shared_ptr__ type can now be used as a dispatched
  parameter, without further alteration of the __multimethod__ syntax.
 */
using tags::_v;
BOOST_MMETHOD_DECLARE(f1, int, (_v<boost::shared_ptr<foo> const&>));

/*`
  The __shared_ptr__ type can also be used on the specialized function.
  [note
    Any type can be passed as a receiving argument in the specializations.
    The type must be a registered pointer type from __rtti__ point of view.
    This type will be passed without modification as the `U` template parameter
    to the `pointer_traits<>::cast<>()` function.
  ]
 */
BOOST_MMETHOD_IMPLEMENT(f1, int, (boost::shared_ptr<foo> const& a)) { return a->f(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (boost::shared_ptr<bar> const& a)) { return a->g(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (boost::shared_ptr<baz> const& a)) { return 2 * a->f(); }
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

  BOOST_CHECK_EQUAL( f1(f),  5 );
  BOOST_CHECK_EQUAL( f1(r), 42 );
  BOOST_CHECK_EQUAL( f1(z), 10 );
  BOOST_CHECK_EQUAL( f1(l), 42 ); // (lap is-a bar)
  //]
}
