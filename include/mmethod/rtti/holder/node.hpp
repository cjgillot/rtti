//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HOLDER_NODE_HPP
#define RTTI_HOLDER_NODE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rttifwd.hpp"

#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/type_with_alignment.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace mmethod {
namespace holder_detail {

template<size_t N>
struct rtti_node_var {
  std::size_t __arity;
  const rtti_node* __base [ N ];
};
template<>
struct rtti_node_var<0> {
  std::size_t __arity;
};

} // namespace holder_detail

struct detail::rtti_node {
  struct alignment;

  union {
    boost::type_with_alignment<2> __alignment;
    holder_detail::rtti_node_var<1> self;
  };
};

inline rtti_type
detail::rtti_get_id(rtti_node const* n)
{ return static_cast<rtti_type>( static_cast<void const*>(n) ); }

inline rtti_node const*
detail::rtti_get_base(rtti_node const* n, std::size_t k)
{
  BOOST_ASSERT(n);
  BOOST_ASSERT(k < n->self.__arity);
  return n->self.__base[k];
}

inline std::size_t
detail::rtti_get_base_arity(rtti_node const* n)
{
  BOOST_ASSERT(n);
  return n->self.__arity;
}

struct detail::rtti_node::alignment
: boost::alignment_of<holder_detail::rtti_node_var<0> >
{};

BOOST_STATIC_ASSERT((detail::rtti_node::alignment::value >= 2) && "Broken invariant");

}} // namespace boost::mmethod

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
