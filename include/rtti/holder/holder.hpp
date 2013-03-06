#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include <type_traits>

#include "rtti/rttifwd.hpp"
#include "rtti/holder/getter.hpp"

#include "util/assert.hpp"

namespace rtti {
namespace detail {
namespace holder_ {

template<typename> struct holder;

//! \brief Grant access to the holder
template<class T>
struct get_holder {
  typedef holder<const volatile T> type;
};

//! \brief Placeholder struct used for hierarchy base
template<>
struct get_holder<const volatile void> {
//   typedef typename RTTI_GETTER::traits<T>::base Rt;
  struct type {
    static constexpr rtti_node* get_node() { return nullptr; }
  };
};

/*
 * Current implementation :
 *
 * rtti_type is a native unsigned integer,
 * which range allows for at least 2 ^ 16 classes
 * in a hierarchy.
 *
 * A new class id is just its name's string hash value.
 */

//! Arguments must be const-qualified to avoid unnecessary instanciations
template<class T>
struct holder {
private:
  static_assert( std::is_const<T>::value && std::is_volatile<T>::value
  , "rtti::detail::holder_::holder<> must not be accessed directly" );

  typedef typename RTTI_GETTER::traits<T>::super super;
  typedef typename get_holder<super>::type sholder;

public:
  static constexpr rtti_node node = {
    rtti_type( rtti_getter::traits<T>::hash )
  , sholder::get_node()
  };

  static constexpr inline
  const rtti_node*
  ATTRIBUTE_PURE
  get_node() noexcept
  { return &node; }

  static constexpr inline
  rtti_type
  ATTRIBUTE_PURE
  get_id() noexcept
  { return node.id; }
};

template<class T>
constexpr rtti_node holder<T>::node;

} // namespace holder_

using holder_::get_holder;

}} // namespace rtti::detail

#endif

// #include "rtti/holder.ipp"
