//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_INTERFACE_HPP
#define RTTI_INTERFACE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rtti/getter/getter.hpp"
#include "boost/mmethod/rtti/getter/getter.ipp"

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>

/*
 * In this file, the [pure] attribute is
 * overused on purpose to create room for
 * as much optimization as possible.
 */

namespace boost {
namespace mmethod {

//! \brief RTTI id type
using detail::rtti_type;

//! \brief Function checking exact type
template<class T, class U>
inline bool BOOST_MMETHOD_ATTRIBUTE_PURE
is_exactly_a(const U &x) {
  return boost::mmethod::static_id<T>() == boost::mmethod::get_id(x);
}

//! \brief Function checking exact type, pointer version
template<class T, class U>
inline bool BOOST_MMETHOD_ATTRIBUTE_PURE
is_exactly_a(const U* x) {
  // reject pointer-to-pointer
  BOOST_STATIC_ASSERT_MSG( ! boost::is_pointer<U>::value, "is_exactly_a<> called with pointer to pointer" );
  typedef typename boost::remove_pointer<T>::type T2;

  // retry without pointer
  return boost::mmethod::is_exactly_a<T2, U>( *x );
}

}} // namespace boost::mmethod

#endif
