#include "../dynamic/early.hpp"
#include "overloads.hpp"

static std::size_t
rerank(
  pole_table_t const& table
) {
  std::size_t incr = 1;

  for(auto const& h : table) {
    std::size_t current = 0;

    for(klass_t const* k : h) {
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
  for(auto const& h : table) {
    for(klass_t const* k : h) {
      dispatch_t::mapped_type const& target = dispatch.at( *k->sig );
      const_cast<klass_t*>( k )->rankhash = reinterpret_cast<uintptr_t>(target ? target->second : &BAD_DISPATCH);
    }
  }
}

static void
make_assignment(
  signature_t const& sig,
  invoker_t  inv,
  invoker_t* table
) {
  std::size_t const index = std::accumulate(
    sig.array().begin(), sig.array().end(), 0,
    [](std::size_t acc, klass_t const* k)
    { return acc + k->rankhash; }
  );

  table[index] = inv;
}

/* Generates the hash function and the key word recognizer function.  */
void gen_mph(
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const early_bindings_type& decl,
  seal_table_type& f
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
    for(auto const& p : dispatch)
      if(p.second)
        make_assignment(p.first, p.second->second, table);
  }
}
