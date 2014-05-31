//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[sm_smart_ptr
/*`
  In order to demonstrate the use of smart pointers 
  with __mmethod__, here is a example of dispatch through __shared_ptr__.
  [sm_specialize]
  
  [sm_declare]
  
  [sm_invoke]
 */
//]

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

//[sm_specialize
/*`
  [heading Enabling __shared_ptr__ in __rtti__]
  
  Before declaring any __multimethod__, the __shared_ptr__
  type must be registered with __rtti__. This way, the __rtti__ subsystem
  will be able to retrieve the type informations and to downcast
  the __shared_ptr__.
  
  This is done via a specialization of the `__rtti_ns__::pointer_traits` structure.
 */
#include <boost/shared_ptr.hpp>
#include <boost/cast.hpp>

namespace rtti {

// begin partial specialization
template<typename T>
struct pointer_traits<boost::shared_ptr<T> > {
  typedef boost::shared_ptr<T> pointer_type;                    // pointer being manipulated

  typedef typename boost::remove_cv<T>::type class_type;        // class being pointed to

  static T&     get(pointer_type const& v) { return *v; }       // fetch the pointed value
  static bool valid(pointer_type const& v) { return bool(v); }  // check the pointer validity

  /*`
    The following method is used by the __mmethod__ system
    to downcast the passed pointers.

    The template parameter is the target pointer type.
    The function argument is the source pointer.
    `cast<U>(p)` must return a pointer of type `U`, with value
    the downcasted pointer `p`.
   */
  template<typename U>
  static typename rtti::traits_detail::remove_all<U>::type
  cast(pointer_type const& v) {
    typedef typename rtti::traits_detail::remove_all<U>::type Uclass;
    typedef typename Uclass::element_type O;
    return Uclass(v, boost::polymorphic_downcast<O*>(v.get()));
  }
};

} // namespace rtti
//]

namespace {

using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
  
  int f() { return 5; }
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{
  int g() { return 42; }
};

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};

//[sm_declare
/*`
  The __shared_ptr__ type can now be used as a dispatched
  parameter, without further alteration of the __multimethod__ syntax.
 */
using tags::_v;
DECLARE_MMETHOD(f1, int, (_v<boost::shared_ptr<foo> const&>));

/*`
  The __shared_ptr__ type can also be used on the specialized function.
  [note
    Any type can be passed as a receiving argument in the specializations.
    The type must be a registered pointer type from __rtti__ point of view.
    This type will be passed without modification as the `U` template parameter
    to the `pointer_traits<>::cast<>()` function.
  ]
 */
IMPLEMENT_MMETHOD(f1, int, (boost::shared_ptr<foo> const& a)) { return a->f(); }
IMPLEMENT_MMETHOD(f1, int, (boost::shared_ptr<bar> const& a)) { return a->g(); }
IMPLEMENT_MMETHOD(f1, int, (boost::shared_ptr<baz> const& a)) { return 2 * a->f(); }
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(smart_ptr) {
  //[sm_invoke
  /*`
    Invocation of the __multimethod__ is also transparent
    for the user.
   */
  boost::shared_ptr<foo> f ( new foo );
  boost::shared_ptr<bar> r ( new bar );
  boost::shared_ptr<baz> z ( new baz );
  boost::shared_ptr<lap> l ( new lap );

  BOOST_CHECK_EQUAL( f1(f),  5 );
  BOOST_CHECK_EQUAL( f1(r), 42 );
  BOOST_CHECK_EQUAL( f1(z), 10 );
  BOOST_CHECK_EQUAL( f1(l), 42 ); // (lap is-a bar)
  //]
}
