//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HOLDER_NODE_HPP
#define RTTI_HOLDER_NODE_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"

#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/type_with_alignment.hpp>

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

} // namespace holder_
} // namespace detail
} // namespace rtti

struct rtti::detail::rtti_node {
  struct alignment;

  union {
    boost::type_with_alignment<MMETHOD_NODE_ALIGN>::type __alignment;
    detail::holder_::rtti_node_var<3> self;
  };

  BOOST_DELETED_FUNCTION( rtti_node() )
};

inline rtti::rtti_type
rtti::detail::rtti_get_id(rtti_node const* n)
{ return static_cast<rtti_type>( static_cast<void const*>(n) ); }

inline rtti::rtti_node const*
rtti::detail::rtti_get_node(rtti_type n)
{ return static_cast<rtti_node const*>( static_cast<void const*>(n) ); }

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

struct rtti::detail::rtti_node::alignment {
  enum { value = boost::alignment_of<rtti_node>::value };

  BOOST_STATIC_ASSERT_MSG(value >= MMETHOD_NODE_ALIGN, "Unsufficient alignment");
};

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
