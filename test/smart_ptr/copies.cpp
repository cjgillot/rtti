//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// The goal of this test is to evaluate the number
// of copies of the pointer object are needed for a call.
#include "../classes.hpp"

#include <boost/cast.hpp>
#include <boost/test/unit_test.hpp>

using namespace rtti::mmethod;

template<typename T>
class count_pointer {
public:
  // Constructor taking pointer
  count_pointer(T* p, int& nc): px(p), ncalls(nc) {}
  template<typename U>
  count_pointer(count_pointer<U> const& v, T* p)
  : px(p), ncalls(v.ncalls) {}

public:
  // Destructor: do nothing (no-op pointer)
  ~count_pointer() {/**/}

public:
  // Copy
  count_pointer(count_pointer const& p)
  : px(p.px), ncalls(p.ncalls) { ++ncalls; }
  BOOST_DELETED_FUNCTION(count_pointer& operator=(count_pointer const&))

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
public:
  // Move
  count_pointer(count_pointer&& p)
  : px(p.px), ncalls(p.ncalls) { p.px = NULL; }
  BOOST_DELETED_FUNCTION(count_pointer& operator=(count_pointer&&))
#endif

public:
  // Access
  typedef T element_type;
  bool valid()    const { return !!px; }

  T* get()        const { return   px; }
  T& operator* () const { return  *px; }
  T* operator->() const { return   px; }

public:
  // Destruction
  void destroy() {
    delete px;
    px = NULL;
  }

private:
  T* px;
  int& ncalls;

  template<typename U>
  friend class count_pointer;
};

namespace rtti {

// begin partial specialization
template<typename T>
struct pointer_traits<count_pointer<T> > {
  typedef count_pointer<T> pointer_type;                           // pointer being manipulated

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
    return Uclass(v, boost::polymorphic_downcast<O*>(v.get()));
  }
};

} // namespace rtti
//]

namespace {

using tags::_v;
DECLARE_MMETHOD(cnt, int, (_v<count_pointer<foo> const&>));

IMPLEMENT_MMETHOD(cnt, int, (count_pointer<foo> const& a)) { return a->f(); }
IMPLEMENT_MMETHOD(cnt, int, (count_pointer<bar> const& a)) { return a->g(); }
IMPLEMENT_MMETHOD(cnt, int, (count_pointer<baz> const& a)) { return 2 * a->f(); }

} // namespace

#include <iostream>

BOOST_AUTO_TEST_CASE(test_copies) {
  int cnt_foo = 0, cnt_bar = 0, cnt_baz = 0, cnt_lap = 0;
  count_pointer<foo> f ( new foo, cnt_foo );
  count_pointer<foo> r ( new bar, cnt_bar );
  count_pointer<foo> z ( new baz, cnt_baz );
  count_pointer<foo> l ( new lap, cnt_lap );

  BOOST_CHECK_EQUAL( cnt(f),  5 );
  BOOST_CHECK_EQUAL( cnt(r), 42 );
  BOOST_CHECK_EQUAL( cnt(z), 10 );
  BOOST_CHECK_EQUAL( cnt(l), 42 ); // (lap is-a bar)

  f.destroy();
  r.destroy();
  z.destroy();
  l.destroy();

  BOOST_WARN_EQUAL( cnt_foo, 0 );
  BOOST_WARN_EQUAL( cnt_bar, 0 );
  BOOST_WARN_EQUAL( cnt_baz, 0 );
  BOOST_WARN_EQUAL( cnt_lap, 0 );
}
