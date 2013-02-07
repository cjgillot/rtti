#ifndef RTTI_GETTER_HPP
#define RTTI_GETTER_HPP

#include <utility>
#include <type_traits>

#include "util/attribute.hpp"

#include "rtti/rttifwd.hpp"

namespace rtti {
namespace detail {

//! \brief Friend structure for in-class rtti access
struct rtti_getter {

  template<class T>
  struct mixin {
    typedef decltype( rtti_get_mixin( std::declval<T const volatile&>() ) ) type;
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
  static rtti_node const* const&
  get_node_pointer(T&& x) noexcept;
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
static_node() noexcept
{ return detail::rtti_getter::static_node<T>(); }

//! \brief Get static id
template<class T>
inline constexpr rtti_type
ATTRIBUTE_CONST
static_id() noexcept
{ return static_node<T>()->id; }

//! \brief Get object node
template<class T>
inline rtti_node const*
ATTRIBUTE_CONST
get_node(T&& x) noexcept
{ return detail::rtti_getter::get_node_pointer(std::forward<T>(x)); }

//! \brief Get pointer node
template<class T>
inline rtti_node const*
ATTRIBUTE_CONST
ATTRIBUTE_NONNULL(1)
get_node(T* x)
{
#if __EXCEPTIONS__
  if(!x) throw std::bad_typeid();
#endif
  return get_node(*x);
}

//! \brief Get object id
template<class T>
static inline rtti_type
ATTRIBUTE_CONST
get_id(T&& x) throw()
{ return get_node(std::forward<T>(x))->id; }

//! \brief Get pointer id
template<class T>
static inline rtti_type
ATTRIBUTE_CONST
ATTRIBUTE_NONNULL(1)
get_id(T* x)
{ return get_node(x)->id; }

} // namespace rtti

#endif
