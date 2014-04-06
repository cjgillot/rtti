//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_GETTER_IPP
#define RTTI_GETTER_IPP

#include "boost/mmethod/config.hpp"

#include "boost/mmethod/rtti/getter/getter.hpp"
#include "boost/mmethod/rtti/getter/traits.hpp"

#include "boost/mmethod/rtti/holder/holder.hpp"
#include "boost/mmethod/rtti/mixin/mixin_node.hpp"

namespace boost {
namespace mmethod {
namespace detail {

template<class T>
BOOST_CONSTEXPR rtti_node const*
rtti_getter::static_node() {
  BOOST_STATIC_ASSERT( boost::is_class<T>::value );
  typedef typename get_holder<T>::type h;
  return h::get_node();
}

template<class T>
inline rtti_node const&
rtti_getter::get_node_value(T const& x) BOOST_NOEXCEPT_OR_NOTHROW {
  typedef rtti_getter::traits<T> traits;
  return *traits::get_mixin_node(x).rtti_node_value;
}

}}} // namespace boost::mmethod::detail

#endif
