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
