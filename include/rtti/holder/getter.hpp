#ifndef RTTI_GETTER_HPP
#define RTTI_GETTER_HPP

#include <utility>

#include "rtti/rttifwd.hpp"
#include "rtti/detail/traits.hpp"
#include "rtti/detail/attribute.hpp"

namespace rtti {
namespace detail {

//! \brief Friend structure for in-class rtti access
struct rtti_getter {

  //! \brief Get static node
  template<class T>
  static BOOST_CONSTEXPR rtti_node const*
  static_node();

  //! \brief Get object node
  template<class T>
  static rtti_node const&
  get_node_value(T const& x) BOOST_NOEXCEPT_OR_NOTHROW;
};

} // namespace detail

//! \brief Get static node
template<class T>
inline BOOST_CONSTEXPR rtti_node const* ATTRIBUTE_PURE
ATTRIBUTE_PURE
static_node() BOOST_NOEXCEPT_OR_NOTHROW {
  typedef rtti::pointer_traits<T> traits;
  return detail::rtti_getter::static_node<typename traits::class_type>();
}

//! \brief Get static id
template<class T>
inline BOOST_CONSTEXPR rtti_type
ATTRIBUTE_CONST
static_id() BOOST_NOEXCEPT_OR_NOTHROW
{ return detail::rtti_get_id( rtti::static_node<T>() ); }

//! \brief Get pointer node
template<class U>
inline rtti_node const*
ATTRIBUTE_PURE
get_node(U& x)
{
  typedef rtti::pointer_traits<U&> traits;
#if __EXCEPTIONS__
  if(! traits::valid(x)) throw std::bad_typeid();
#endif
  return &detail::rtti_getter::get_node_value( traits::get(x) );
}

//! \brief Get pointer node
template<class U>
inline rtti_node const*
ATTRIBUTE_PURE
get_node(U const& x)
{
  typedef rtti::pointer_traits<U const&> traits;
#if __EXCEPTIONS__
  if(! traits::valid(x)) throw std::bad_typeid();
#endif
  return &detail::rtti_getter::get_node_value( traits::get(x) );
}

//! \brief Get object id
template<class U>
inline rtti_type
ATTRIBUTE_PURE
get_id(U& x) BOOST_NOEXCEPT_OR_NOTHROW
{ return detail::rtti_get_id( rtti::get_node(x) ); }

//! \brief Get object id
template<class U>
inline rtti_type
ATTRIBUTE_PURE
get_id(U const& x) BOOST_NOEXCEPT_OR_NOTHROW
{ return detail::rtti_get_id( rtti::get_node(x) ); }

} // namespace rtti

#endif
