//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HOLDER_NODE_HPP
#define RTTI_HOLDER_NODE_HPP

#include "mmethod/rttifwd.hpp"

#include <boost/assert.hpp>
#include <boost/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

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

struct rtti::detail::rtti_node {
  detail::holder_::rtti_node_var<1> self;
};

inline rtti::rtti_type
rtti::detail::rtti_get_id(rtti_node const* n)
{ return (void*)n; }

inline rtti::rtti_node const*
rtti::detail::rtti_get_base(rtti_node const* n, std::size_t k)
{
  BOOST_ASSERT(n);
  BOOST_ASSERT(k < n->self.__arity);
  return n->self.__base[k];
}

inline std::size_t
rtti::detail::rtti_get_base_arity(rtti_node const* n)
{
  BOOST_ASSERT(n);
  return n->self.__arity;
}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
