//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include <vector>

#include "hierarchy.hpp"
#include "overloads.hpp"

#include "early.hpp"

#define USE_SMALLARRAY 0


void rtti_dispatch::order_poles(
  pole_table_t& pole_table
, std::vector<std::vector<rtti_hierarchy> > const& hierarchies
) {
  std::size_t const arity = pole_table.size();

  for(std::size_t i = 0; i < arity; ++i) {
    pole_table_t::reference t = pole_table[i];
    std::vector<rtti_hierarchy> const& h = hierarchies[i];

    t.compute_poles(h);
  }
}
