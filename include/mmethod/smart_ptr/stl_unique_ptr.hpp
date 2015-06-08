//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_STD_UNIQUE_PTR_HPP
#define BOOST_MMETHOD_STD_UNIQUE_PTR_HPP

#include "mmethod/traits/pointer_traits.hpp"

#ifndef BOOST_NO_CXX11_SMART_PTR

#include <memory>

namespace rtti {

template<typename T, typename Del>
struct pointer_traits<std::unique_ptr<T, Del> > {
  typedef std::unique_ptr<T, Del> pointer_type;

  typedef typename boost::remove_cv<T>::type class_type;

  static T&     get(pointer_type const& v) { return *v; }
  static bool valid(pointer_type const& v) { return bool(v); }

private:
  template<typename Out>
  struct cast_detail {};

public:
  template<typename Out>
  static typename traits_detail::remove_all<Out>::type
  cast(pointer_type&& v) {
    typedef typename traits_detail::remove_all<Out>::type OutClass;
    return cast_detail<OutClass>::cast(std::move(v));
  }
};

template<typename T, typename DT>
template<typename U, typename DU>
struct pointer_traits<std::unique_ptr<T, DT> >::
cast_detail<std::unique_ptr<U, DU> > {

  typedef std::unique_ptr<U, DU> OutClass;
  typedef typename boost::add_pointer<U>::type Uptr;

  static OutClass cast(pointer_type&& v) {
    Uptr up = traits_detail::unsafe_casting<Uptr>::eval(v.get());
    OutClass ret ( up );
    v.release();
    return ret;
  }
};

} // namespace rtti

#endif // BOOST_NO_CXX11_SMART_PTR

#endif
