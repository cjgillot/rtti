//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_INTERFACE_HPP
#define RTTI_INTERFACE_HPP

#include <boost/type_traits.hpp>

#include <boost/static_assert.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include "mmethod/rtti/holder/getter.ipp"

#include "mmethod/detail/attribute.hpp"

/*!
 * \brief Macros controlling the is_a behaviour
 * \{
 */
//! Choose whether \ref rtti::is_a should
//! use built-in \c dynamic_cast<> operator,
//! or prefer reimplementing it in terms of \c rtti_node
#define RTTI_IS_A_USE_DYNAMIC_CAST
//! Choose whether \ref rtti::is_a should
//! use \ref rtti::is_exactly_a when the target
//! is flagged as a final class
#define RTTI_IS_A_USE_FINAL
/*!\}*/

/*
 * In this file, the [pure] attribute is
 * overused on purpose to create room for
 * as much optimization as possible.
 */

namespace rtti {

//! \brief RTTI id type
using detail::rtti_type;

//! \brief Function checking exact type
template<class T, class U>
inline bool MMETHOD_ATTRIBUTE_PURE
is_exactly_a(const U &x) {
  return rtti::static_id<T>() == rtti::get_id(x);
}

//! \brief Function checking exact type, pointer version
template<class T, class U>
inline bool MMETHOD_ATTRIBUTE_PURE
is_exactly_a(const U* x) {
  // reject pointer-to-pointer
  BOOST_STATIC_ASSERT_MSG( ! boost::is_pointer<U>::value, "is_exactly_a<> called with pointer to pointer" );
  typedef typename boost::remove_pointer<T>::type T2;

  // retry without pointer
  return rtti::is_exactly_a<T2, U>( *x );
}

} // namespace rtti

#endif
