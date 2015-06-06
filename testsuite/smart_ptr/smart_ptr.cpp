//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[sm_smart_ptr
/*`
  In order to demonstrate the use of smart pointers
  with __mmethod__, here is a example of dispatch through
  a custom smart pointer.

  [sm_pointer]
  [sm_specialize]
  [sm_declare]
  [sm_invoke]
 */
//]

#include "../classes.hpp"

#include <boost/test/unit_test.hpp>

using namespace rtti::mmethod;

//[sm_pointer
/*`
  In this section, we will register a toy smart pointer
  for use with __rtti__.
  The pointer type is given here:
 */
template<typename T>
class noop_pointer {
public:
  // Constructor taking pointer
  noop_pointer(): px(NULL) {}
  noop_pointer(T* p): px(p) {}

  // Destructor: do nothing (no-op pointer)
  ~noop_pointer() {/**/}

  // Copy
  noop_pointer(noop_pointer const& p)
  : px(p.px) {}
  noop_pointer& operator=(noop_pointer const& p) {
    px = p.px;
    return *this;
  }

  // Access
  typedef T element_type;
  bool valid()    const { return !!px; }

  T* get()        const { return   px; }
  T& operator* () const { return  *px; }
  T* operator->() const { return   px; }

  // Destruction
  void destroy() {
    delete px;
    px = NULL;
  }

private:
  T* px;
};
//]

//[sm_specialize
/*`
  Before declaring any __multimethod__, the `noop_pointer`
  type must be registered with __rtti__. This way, the __rtti__ subsystem
  will be able to retrieve the type informations and to downcast
  the `noop_pointer`.

  This is done via a specialization of the `__rtti_ns__::pointer_traits` structure.
 */
#include <boost/cast.hpp>

namespace rtti {

// begin partial specialization
template<typename T>
struct pointer_traits<noop_pointer<T> > {
  typedef noop_pointer<T> pointer_type;                           // pointer being manipulated

  typedef typename boost::remove_cv<T>::type class_type;          // class being pointed to

  static T&     get(pointer_type const& v) { return *v; }         // fetch the pointed value
  static bool valid(pointer_type const& v) { return v.valid(); }  // check the pointer validity

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
    return Uclass(boost::polymorphic_downcast<O*>(v.get()));
  }
};

} // namespace rtti
//]

namespace {

//[sm_declare
/*`
  The __shared_ptr__ type can now be used as a dispatched
  parameter, without further alteration of the __multimethod__ syntax.
 */
using tags::_v;
DECLARE_MMETHOD(smp, int, (_v<noop_pointer<foo> const&>));

/*`
  The __shared_ptr__ type can also be used on the specialized function.
  [note
    Any type can be passed as a receiving argument in the specializations.
    The type must be a registered pointer type from __rtti__ point of view.
    This type will be passed without modification as the `U` template parameter
    to the `pointer_traits<>::cast<>()` function.
  ]
 */
IMPLEMENT_MMETHOD(smp, int, (noop_pointer<foo> const& a)) { return a->f(); }
IMPLEMENT_MMETHOD(smp, int, (noop_pointer<bar> const& a)) { return a->g(); }
IMPLEMENT_MMETHOD(smp, int, (noop_pointer<baz> const& a)) { return 2 * a->f(); }
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_smart_ptr) {
  //[sm_invoke
  /*`
    Invocation of the __multimethod__ is also transparent
    for the user.
   */
  noop_pointer<foo> f ( new foo );
  noop_pointer<foo> r ( new bar );
  noop_pointer<foo> z ( new baz );
  noop_pointer<foo> l ( new lap );

  BOOST_CHECK_EQUAL( smp(f),  5 );
  BOOST_CHECK_EQUAL( smp(r), 42 );
  BOOST_CHECK_EQUAL( smp(z), 10 );
  BOOST_CHECK_EQUAL( smp(l), 42 ); // (lap is-a bar)

  f.destroy();
  r.destroy();
  z.destroy();
  l.destroy();
  //]
}
