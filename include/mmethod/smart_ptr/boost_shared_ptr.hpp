//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_BOOST_SHARED_PTR_HPP
#define BOOST_MMETHOD_BOOST_SHARED_PTR_HPP

#include "boost/mmethod/traits/pointer_traits.hpp"

#include <boost/shared_ptr.hpp>

namespace boost {
namespace mmethod {

template<typename T>
struct pointer_traits<boost::shared_ptr<T> > {
  typedef boost::shared_ptr<T> pointer_type;                    // pointer being manipulated

  typedef typename boost::remove_cv<T>::type class_type;        // class being pointed to

  static T&     get(pointer_type const& v) { return *v; }       // fetch the pointed value
  static bool valid(pointer_type const& v) { return bool(v); }  // check the pointer validity

private:
  template<typename Out>
  struct cast_detail {};

public:
  template<typename Out>
  static typename traits_detail::remove_all<Out>::type
  cast(pointer_type const& v) {
    typedef typename traits_detail::remove_all<Out>::type OutClass;
    return cast_detail<OutClass>::cast(v);
  }
};

template<typename T>
template<typename U>
struct pointer_traits<boost::shared_ptr<T> >::cast_detail<boost::shared_ptr<U> > {

  typedef boost::shared_ptr<U> OutClass;
  typedef typename boost::add_pointer<U>::type Uptr;

  static OutClass cast(pointer_type const& v) {
    return OutClass(
      v, traits_detail::unsafe_casting<Uptr>::eval(v.get())
    );
  }
};

}} // namespace boost::mmethod

#endif