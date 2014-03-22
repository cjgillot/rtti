//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_GETTER_IPP
#define RTTI_GETTER_IPP

#include "mmethod/rtti/holder/getter.hpp"
#include "mmethod/rtti/holder/holder.hpp"
#include "mmethod/traits/pointer_traits.hpp"

#include "mmethod/rtti/mixin/mixin_node.hpp"

namespace rtti {
namespace detail {

template<class T>
BOOST_CONSTEXPR rtti_node const*
rtti_getter::static_node() {
  typedef typename traits_detail::remove_all<T>::type T2;
  typedef typename get_holder<T2>::type h;
  return h::get_node();
}

template<class T>
inline rtti_node const&
rtti_getter::get_node_value(T const& x) BOOST_NOEXCEPT_OR_NOTHROW {
  return *rtti_get_mixin(x).detail::mixin_node::rtti_node_value;
}

}} // namespace rtti::detail

#endif
