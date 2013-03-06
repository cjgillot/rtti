#ifndef RTTI_GETTER_IPP
#define RTTI_GETTER_IPP

#include "rtti/holder/getter.hpp"
#include "rtti/holder/holder.hpp"

#include <boost/type_traits/remove_pointer.hpp>

namespace rtti {
namespace detail {

template<class T>
constexpr rtti_node const*
rtti_getter::static_node() {
  typedef typename std::remove_pointer<T>::type T2;
  typedef typename get_holder<T2>::type h;
  return h::get_node();
}

template<class T>
inline rtti_node const&
rtti_getter::get_node_value(T&& x) noexcept {
  typedef RTTI_GETTER::traits<T> traits;
  typedef typename RTTI_GETTER::mixin<typename traits::root>::type root_mixin;
  return static_cast<root_mixin const&&>(x).rtti_node_value;
}

}} // namespace rtti::detail

#endif
