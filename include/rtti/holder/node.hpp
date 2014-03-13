//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HOLDER_NODE_HPP
#define RTTI_HOLDER_NODE_HPP

#include "rtti/rttifwd.hpp"

namespace rtti {
namespace detail {
namespace holder_ {

template<size_t N>
struct rtti_node_var {
  std::size_t __arity;
  const rtti_node* __base [ N ];
};
template<>
struct rtti_node_var<0> {
  std::size_t __arity;
};

}}} // namespace rtti::detail::holder_

#endif
