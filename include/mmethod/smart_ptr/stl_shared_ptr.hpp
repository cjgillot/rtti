//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_STL_SHARED_PTR_HPP
#define BOOST_MMETHOD_STL_SHARED_PTR_HPP

#include "mmethod/traits/pointer_traits.hpp"

#ifndef BOOST_NO_CXX11_SMART_PTR

#include <memory>

namespace rtti {

template<typename T>
struct pointer_traits<std::shared_ptr<T> > {
  typedef std::shared_ptr<T> pointer_type;

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
struct pointer_traits<std::shared_ptr<T> >::
cast_detail<std::shared_ptr<U> > {

  typedef std::shared_ptr<U> OutClass;
  typedef typename boost::add_pointer<U>::type Uptr;

  static OutClass cast(pointer_type const& v) {
    Uptr up = traits_detail::unsafe_casting<Uptr>::eval(v.get());
    return OutClass(v, up);
  }
};

} // namespace rtti

#endif // BOOST_NO_CXX11_SMART_PTR

#endif
