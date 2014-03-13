//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MANIP_HPP
#define RTTI_MANIP_HPP

#include "rtti/rttifwd.hpp"
#include "rtti/holder/node.hpp"

namespace rtti {

struct detail::rtti_node
{
  detail::holder_::rtti_node_var<1> self;
};

inline rtti_node const*
rtti_get_base(rtti_node const* n, std::size_t k = 0)
{ return n->self.__base[k]; }

inline std::size_t
rtti_get_base_arity(rtti_node const* n)
{ return n->self.__arity; }

}

#endif
