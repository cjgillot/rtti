//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EARLY_HPP
#define EARLY_HPP

#include "mmethod/export/table.hpp"

#include <vector>

using rtti::rtti_type;
using rtti::rtti_hierarchy;
using rtti::rtti_get_base;
using rtti::rtti_get_base_arity;

using rtti::mmethod::detail::invoker_t;
using rtti::mmethod::detail::early_bindings_struct;

typedef std::vector<rtti_hierarchy>          rtti_signature;
typedef std::pair<rtti_signature, invoker_t> rtti_binding;
struct rtti::mmethod::detail::early_bindings_struct {
  std::size_t arity;
  std::vector<rtti_binding> vector;
};

using rtti::mmethod::detail::poles_map_type;
using rtti::mmethod::detail::seal_table_type;
using rtti::mmethod::detail::ambiguity_handler_t;

namespace rtti_dispatch {
  // dispatch/mmethod.cpp
  extern void process_declaration(early_bindings_struct const&, seal_table_type&);
} // namespace rtti_dispatch

#endif
