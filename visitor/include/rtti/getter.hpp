#ifndef RTTI_GETTER_HPP
#define RTTI_GETTER_HPP

#include <typeinfo>
#include <type_traits>

#include "util/attribute.hpp"

#include "rtti/rttifwd.hpp"

namespace rtti {
namespace detail {

//! \brief Friend structure for in-class rtti access
struct rtti_getter {

  template<class T>
  struct mixin {
    typedef decltype( rtti_get_mixin( std::declval<T const&>() ) ) type;
  };

  template<class T>
  struct traits
  : public mixin<T>::type::rtti_traits
  {};

  //! \brief Get static node
  template<class T>
  static inline const rtti_node*
  ATTRIBUTE_PURE
  static_node() noexcept;

  //! \brief Get object node
  template<class T>
  static inline rtti_node const* const&
  ATTRIBUTE_PURE
  get_node_pointer(T&& x) noexcept;
};

} // namespace detail

template<class T>
static constexpr inline rtti_type
type_hash() noexcept
{ return detail::rtti_getter::traits<T>::hash; }

//! \brief Get static node
template<class T>
inline rtti_node const*
ATTRIBUTE_PURE
static_node() noexcept
{ return detail::rtti_getter::static_node<T>(); }

//! \brief Get static id
template<class T>
inline rtti_type
ATTRIBUTE_PURE
static_id() noexcept
{ return static_node<T>()->id; }

//! \brief Get object node
template<class T>
inline rtti_node const*
ATTRIBUTE_PURE
get_node(T&& x) noexcept
{ return detail::rtti_getter::get_node_pointer(std::forward<T>(x)); }

//! \brief Get pointer node
template<class T>
inline rtti_node const*
ATTRIBUTE_PURE
ATTRIBUTE_NONNULL(1)
get_node(T* x) throw( std::bad_typeid )
{
#if __EXCEPTIONS__
  if(!x) throw std::bad_typeid();
#endif
  return get_node(*x);
}

//! \brief Get object id
template<class T>
static inline rtti_type
ATTRIBUTE_PURE
get_id(T&& x) throw()
{ return get_node(std::forward<T>(x))->id; }

//! \brief Get pointer id
template<class T>
static inline rtti_type
ATTRIBUTE_PURE
ATTRIBUTE_NONNULL(1)
get_id(T* x) throw( std::bad_typeid )
{ return get_node(x)->id; }

} // namespace rtti

#if 0
#define RTTI_GETTER \
  ::rtti::detail::rtti_getter

//! \brief access an object RTTI node
#define RTTI_NODE( x ) \
  ::rtti::get_node( x )

//! \brief access an object RTTI id
#define RTTI_ID( x ) \
  ::rtti::get_id( x )

//! \brief access a type RTTI node
#define RTTI_TYPE_NODE( T ) \
  ::rtti::static_node<T>()

//! \brief access a type RTTI id
#define RTTI_TYPE_ID( T ) \
  ::rtti::static_id<T>()

//! \brief access a type RTTI id
#define RTTI_TYPE_HASH( T ) \
  RTTI_GETTER::traits<T>::hash
#endif

#endif
