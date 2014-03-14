//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "early.hpp"
#include "forward.hpp"

#include <numeric>
#include <boost/foreach.hpp>

static std::size_t
rerank(
  pole_table_t const& table
) {
  std::size_t incr = 1;

  BOOST_FOREACH(pole_table_t::const_reference h, table) {
    std::size_t current = 0;

    BOOST_FOREACH(klass_t const* k, h) {
      const_cast<klass_t*>( k )->rankhash = current;
      current += incr;
    }

    incr = current;
  }

  return incr;
}

static void
rerank_unary(
  pole_table_t const& table
, dispatch_t dispatch
) {
  BOOST_FOREACH(pole_table_t::const_reference h, table) {
    BOOST_FOREACH(klass_t const* k, h) {
      dispatch_t::mapped_type const& target = dispatch.at( *k->sig );
      const_cast<klass_t*>( k )->rankhash = reinterpret_cast<uintptr_t>(target ? target->second : &BAD_DISPATCH);
    }
  }
}

namespace {

struct rankhash_adder {
  std::size_t operator()(std::size_t acc, klass_t const* k)
  { return acc + k->rankhash; }
};

static void
make_assignment(
  signature_t const& sig,
  invoker_t  inv,
  invoker_t* table
) {
  std::size_t const index = std::accumulate(
    sig.array().begin(), sig.array().end(), 0,
    rankhash_adder()
  );

  table[index] = inv;
}

} // namespace <>

/* Generates the hash function and the key word recognizer function.  */
void rtti_dispatch::output_dispatch_table(
  seal_table_type& f,
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const early_bindings_type& decl
) {
  if(decl.arity == 1) {
    rerank_unary(pole_table, dispatch);
  }
  else {
    std::size_t const max_index = rerank(pole_table);

    invoker_t* const table = new invoker_t [ max_index ];
    f.table = table;
    
    std::fill_n(table, max_index, BAD_DISPATCH);
    
    /* Generate an array of reserved words at appropriate locations.  */
    BOOST_FOREACH(dispatch_t::const_reference p, dispatch)
      if(p.second)
        make_assignment(p.first, p.second->second, table);
  }
}
