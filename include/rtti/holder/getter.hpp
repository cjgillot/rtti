#ifndef RTTI_GETTER_HPP
#define RTTI_GETTER_HPP

#include <utility>
#include <type_traits>

#include "util/attribute.hpp"

#include "rtti/rttifwd.hpp"
#include "rtti/traits.hpp"

namespace rtti {
namespace detail {

//! \brief Friend structure for in-class rtti access
struct rtti_getter {

  template<class T>
  struct mixin {
    using type = decltype( rtti_get_mixin( std::declval<T const volatile&>() ) );
  };

  template<class T>
  struct traits
  : public mixin<T>::type::rtti_traits
  {};

  //! \brief Get static node
  template<class T>
  static constexpr rtti_node const*
  static_node();

  //! \brief Get object node
  template<class T>
  static rtti_node const&
  get_node_value(T&& x) noexcept;
};

} // namespace detail

template<class T>
inline constexpr rtti_type ATTRIBUTE_PURE
type_hash() noexcept
{ return rtti_type( detail::rtti_getter::traits<T>::hash ); }

//! \brief Get static node
template<class T>
inline constexpr rtti_node const* ATTRIBUTE_PURE
ATTRIBUTE_PURE
static_node() noexcept {
  typedef rtti::pointer_traits<T> traits;
  return detail::rtti_getter::static_node<typename traits::class_type>();
}

//! \brief Get static id
template<class T>
inline constexpr rtti_type
ATTRIBUTE_CONST
static_id() noexcept
{ return rtti::static_node<T>()->id; }

//! \brief Get pointer node
template<class U>
inline rtti_node const*
ATTRIBUTE_PURE
get_node(U&& x)
{
  typedef rtti::pointer_traits<U&&> traits;
#if __EXCEPTIONS__
  if(! traits::valid(x)) throw std::bad_typeid();
#endif
  return &RTTI_GETTER::get_node_value( traits::get( std::forward<U>(x) ) );
}

//! \brief Get object id
template<class U>
inline rtti_type
ATTRIBUTE_PURE
get_id(U&& x) throw()
{ return rtti::get_node( std::forward<U>(x) )->id; }

} // namespace rtti

#endif
