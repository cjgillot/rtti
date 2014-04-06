//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_GETTER_HPP
#define RTTI_GETTER_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"
#include "mmethod/traits/pointer_traits.hpp"

#include "mmethod/export/exception.hpp"

#include <utility>

namespace rtti {
namespace detail {

//! \brief Friend structure for in-class rtti access
struct rtti_getter {
  //! \brief Traits structure
  template<class M>
  struct traits;

  //! \brief Get static node
  template<class T>
  inline static BOOST_CONSTEXPR rtti_node const*
  static_node();

  //! \brief Get object node
  template<class T>
  inline static rtti_node const&
  get_node_value(T const& x) BOOST_NOEXCEPT_OR_NOTHROW;
};

} // namespace detail

//! \brief Get static node
template<class T>
inline BOOST_CONSTEXPR rtti_node const*
MMETHOD_ATTRIBUTE_PURE
static_node() BOOST_NOEXCEPT_OR_NOTHROW {
  typedef rtti::pointer_traits<T> traits;
  return detail::rtti_getter::static_node<typename traits::class_type>();
}

//! \brief Get static id
template<class T>
inline BOOST_CONSTEXPR rtti_type
MMETHOD_ATTRIBUTE_PURE
static_id() BOOST_NOEXCEPT_OR_NOTHROW
{ return detail::rtti_get_id( rtti::static_node<T>() ); }

//! \brief Get pointer node
template<class U>
inline rtti_node const*
MMETHOD_ATTRIBUTE_PURE
get_node(U& x)
{
  typedef rtti::pointer_traits<U&> traits;
  if(! traits::valid(x)) {
#ifndef BOOST_NO_EXCEPTIONS
    throw bad_rtti();
#else
    std::abort();
#endif
  }
  return &detail::rtti_getter::get_node_value( traits::get(x) );
}

//! \brief Get pointer node
template<class U>
inline rtti_node const*
MMETHOD_ATTRIBUTE_PURE
get_node(U const& x)
{
  typedef rtti::pointer_traits<U const&> traits;
  if(! traits::valid(x)) {
#ifndef BOOST_NO_EXCEPTIONS
    throw bad_rtti();
#else
    std::abort();
#endif
  }
  return &detail::rtti_getter::get_node_value( traits::get(x) );
}

//! \brief Get object id
template<class U>
inline rtti_type
MMETHOD_ATTRIBUTE_PURE
get_id(U& x) BOOST_NOEXCEPT_OR_NOTHROW
{ return detail::rtti_get_id( rtti::get_node(x) ); }

//! \brief Get object id
template<class U>
inline rtti_type
MMETHOD_ATTRIBUTE_PURE
get_id(U const& x) BOOST_NOEXCEPT_OR_NOTHROW
{ return detail::rtti_get_id( rtti::get_node(x) ); }

} // namespace rtti

#endif
