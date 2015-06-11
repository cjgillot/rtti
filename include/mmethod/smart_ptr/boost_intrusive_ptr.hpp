//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_BOOST_INTRUSIVE_PTR_HPP
#define BOOST_MMETHOD_BOOST_INTRUSIVE_PTR_HPP

#include "mmethod/traits/pointer_traits.hpp"

#include <boost/intrusive_ptr.hpp>

namespace rtti {

template<typename T>
struct pointer_traits<boost::intrusive_ptr<T> > {
  typedef boost::intrusive_ptr<T> pointer_type;

  typedef typename boost::remove_cv<T>::type class_type;

  static T&     get(pointer_type const& v) { return *v; }
  static bool valid(pointer_type const& v) { return bool(v); }

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
struct pointer_traits<boost::intrusive_ptr<T> >::
cast_detail<boost::intrusive_ptr<U> > {

  typedef boost::intrusive_ptr<U> OutClass;
  typedef typename boost::add_pointer<U>::type Uptr;

  static OutClass cast(pointer_type const& v) {
    Uptr up = traits_detail::unsafe_casting<Uptr>::eval(v.get());
    return OutClass(up);
  }
};

} // namespace rtti

#endif
