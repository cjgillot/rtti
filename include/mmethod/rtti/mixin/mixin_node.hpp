//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_NODE_HPP
#define RTTI_MIXIN_NODE_HPP

#include "mmethod/rttifwd.hpp"

namespace rtti {
namespace detail {

struct mixin_node {
  rtti::rtti_node const* rtti_node_value;
};

}} // namespace rtti::detail

#endif
