//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[sm_smart_ptr
/*`
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
  In order to demonstrate the use of smart pointers
  with __mmethod__, here is a example of dispatch through
  a custom (dumb) pointer.
  The toy class we want to use is the following:
 */
// Do-nothing wrapper around raw pointer
template<typename T>
class noop_pointer {
public:
  // Constructor taking pointer
  explicit noop_pointer(T* p = NULL): px(p) {/**/}

  // Destructor: no-op
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
  // pointer being manipulated
  typedef noop_pointer<T> pointer_type;

  // class being pointed to
  typedef typename boost::remove_cv<T>::type class_type;

  // check the pointer validity
  static bool valid(pointer_type const& v) { return v.valid(); }

  // fetch the pointed value
  static T& get(pointer_type const& v) { return *v; }

  // downcast the pointer
  template<typename U>
  static typename boost::remove_reference<U>::type
  cast(pointer_type const& v);
};

/*`
  The following method is used by the __mmethod__ system
  to downcast the passed pointers.

  The template parameter is the target pointer type.
  The function argument is the source pointer.
  `cast<U>(p)` must return an object convertible to type `U`,
  with value the pointer `p` casted to the right destination type.
  The type `U` will be the exact type expected by the
  __multimethod__ implementation,
  so it is often a constant reference to `noop_pointer`.
 */
template<typename T>
template<typename U>
typename boost::remove_reference<U>::type
pointer_traits<noop_pointer<T> >::cast(pointer_type const& v)
{
  typedef typename rtti::traits_detail::remove_all<U>::type Uclass;
  typedef typename Uclass::element_type O;
  O* po = boost::polymorphic_downcast<O*>(v.get());
  return Uclass(po);
}

} // namespace rtti
//]

namespace {

//[sm_declare
/*`
  Our `noop_pointer` type can now be used as a dispatched
  parameter, without further alteration of the __multimethod__ syntax.
 */
using tags::_v;
DECLARE_MMETHOD(smp, int, (_v<noop_pointer<foo> const&>));

/*`
  Our pointer type can now be used on the implementation function.
  When a call will be performed,
  the function `pointer_traits<noop_pointer<foo> >::cast<U>`
  will be called,
  with `U` the exact type appearing in the implementation definition.
  Here, the needed instantiations are respectively
  `U = noop_pointer<foo> const&`, `U = noop_pointer<bar> const&`
  and `U = noop_pointer<baz> const&`.
 */
IMPLEMENT_MMETHOD(smp, int, (noop_pointer<foo> const& a)) { return a->f(); }
IMPLEMENT_MMETHOD(smp, int, (noop_pointer<bar> const& a)) { return a->g(); }
IMPLEMENT_MMETHOD(smp, int, (noop_pointer<baz> const& a)) { return 2 * a->f(); }
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(test_smart_ptr) {
  //[sm_invoke
  /*`
    Finally, the invocation of the __multimethod__
    is transparent for the user,
    everything behaves as with a native function taking `noop_pointer<foo>`
    by constant reference.
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
