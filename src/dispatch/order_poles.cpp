//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <vector>

#include "hierarchy.hpp"
#include "overloads.hpp"

#include "early.hpp"

#define USE_SMALLARRAY 0


void order_poles(
  pole_table_t& pole_table
, std::vector<hierarchy_t>& hierarchies
) {
  std::size_t const arity = pole_table.size();

  for(std::size_t i = 0; i < arity; ++i) {
    pole_table_t::reference t = pole_table[i];
    hierarchy_t& h = hierarchies[i];

    h.compute_poles(t);
  }
}

struct print_insert {
  poles_map_type& a;
  std::size_t i;
  std::size_t arity;

  void operator()(klass_t const* k) {
    // insert expects 2-aligned values
    if(arity == 1) {
      // assert code is 2-aligned
      BOOST_ASSERT( (k->rankhash & 1) == 0 );
      a.insert(k->get_id(), k->rankhash);
    }
    else
      a.insert(k->get_id(), 2 * k->rankhash);
  }
};

template<typename Seq>
static void print_map(seal_table_type& output, std::size_t i, Seq const& t, std::size_t arity) {
  /// split \c t between static and dynamic id
  std::vector<klass_t const*> dynamics ( boost::begin(t), boost::end(t) );

  poles_map_type& a = *output.poles[i];

  a.create( dynamics.size() );

  print_insert ins = { a, i, arity };
  std::for_each(dynamics.begin(), dynamics.end(), ins);
}

void print_poles(
  seal_table_type& output
, early_bindings_type const& decl
, pole_table_t const& pole_table
) {
  std::size_t const arity = decl.arity;

  for(std::size_t i = 0; i < arity; ++i) {
    pole_table_t::const_reference t = pole_table[i];
    print_map(output, i, t, arity);
  }
}
