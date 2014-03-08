#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include "rtti/rttifwd.hpp"
#include "rtti/holder/getter.hpp"

#include <boost/assert.hpp>

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
    static BOOST_CONSTEXPR rtti_node* get_node() { return 0; }
  };
};

//! \brief Root holder with the seed
template<typename Root>
struct root_holder {
  template<bool Static>
  static rtti_type make(std::size_t hash) {
    if(Static)
      return rtti_type(hash);

    static std::size_t current = rtti_getter::traits<Root>::static_max;
    return rtti_type(current++);
  }
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
  BOOST_STATIC_ASSERT( boost::is_const<T>::value && boost::is_volatile<T>::value
  && "rtti::detail::holder_::holder<> must not be accessed directly" );

  typedef rtti_getter::traits<T> trts;
  typedef typename trts::root  root;
  typedef typename trts::super super;
  typedef typename get_holder<super>::type sholder;

public:
  static rtti_node node;

  static BOOST_CONSTEXPR inline
  const rtti_node*
  ATTRIBUTE_PURE
  get_node() BOOST_NOEXCEPT_OR_NOTHROW
  { return &node; }

  static inline
  rtti_type
  ATTRIBUTE_PURE
  get_id() BOOST_NOEXCEPT_OR_NOTHROW
  { return node.id; }
};

template<class T>
rtti_node holder<T>::node = {
  root_holder<root>::template make<trts::static_>(trts::hash)
, sholder::get_node()
};

} // namespace holder_

using holder_::get_holder;

}} // namespace rtti::detail

#endif

// #include "rtti/holder.ipp"
