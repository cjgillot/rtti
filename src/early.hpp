//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EARLY_HPP
#define EARLY_HPP

#include "mmethod/export/table.hpp"
#include "mmethod/export/exception.hpp"

#include <vector>

using namespace rtti;
using rtti::mmethod::detail::invoker_t;
using rtti::mmethod::detail::early_bindings_struct;

typedef std::vector<rtti_hierarchy> signature_type;
typedef std::pair<signature_type, invoker_t> binding_type;
struct rtti::mmethod::detail::early_bindings_struct {
  std::size_t arity;
  std::vector<binding_type> vector;
};

using rtti::mmethod::detail::poles_map_type;
using rtti::mmethod::detail::seal_table_type;
using rtti::mmethod::detail::ambiguity_handler_t;

namespace rtti_dispatch {
  // dispatch/mmethod.cpp
  extern void process_declaration(early_bindings_struct const&, seal_table_type&);
}

#endif
