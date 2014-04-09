//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DISPATCH_FORWARD_HPP
#define DISPATCH_FORWARD_HPP

#include "early.hpp"

#include "dispatch/hierarchy.hpp"
#include "dispatch/overloads.hpp"

namespace rtti_dispatch {


// from dispatch.cpp
extern void dispatch(
  dispatch_t &dispatch,
  overloads_t& overloads,
  const pole_table_t &pole_table,
  ambiguity_handler_t ahndl
);

// from order_poles.cpp
extern void order_poles(
  pole_table_t& pole_table,
  std::vector<hierarchy_t>& hierarchies
);

// from mph.cpp
extern void output_tables(
  seal_table_type& f,
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const early_bindings_type& decl
);

} // namespace rtti_dispatch

#endif
