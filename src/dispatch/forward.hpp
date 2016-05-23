//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DISPATCH_FORWARD_HPP
#define DISPATCH_FORWARD_HPP

#include "early.hpp"

#include <vector>

#include <boost/unordered_map.hpp>

class klass_t;
class hierarchy_t;
class signature_t;

typedef std::vector<hierarchy_t> pole_table_t;

// the invoker is null in case of bad dispatch
typedef boost::unordered_map<signature_t, invoker_t> dispatch_t;
typedef dispatch_t::value_type overload_t;

class link_table_t;

namespace rtti_dispatch {

// from dispatch.cpp
extern void dispatch(
  dispatch_t &dispatch,
  const pole_table_t &pole_table,
  ambiguity_handler_t ahndl
);

// from order_poles.cpp
extern void order_poles(
  pole_table_t& pole_table
, std::vector<std::vector<rtti_hierarchy> > const& hierarchies
);

// from mph.cpp
extern void output_tables(
  seal_table_type& f,
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const early_bindings_struct& decl
);

} // namespace rtti_dispatch

#endif
