//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "early.hpp"
#include "forward.hpp"

#include "foreach.hpp"

#include <numeric>

typedef boost::unordered_map<klass_t const*, uintptr_t> hash_table_type;

class output_device {
  seal_table_type& output;

  hash_table_type ht;
  std::size_t max_index;

  pole_table_t const& poles;
  dispatch_t   const& dispatch;

public:
  output_device(seal_table_type& o, pole_table_t const& p, dispatch_t const& d)
  : output(o), poles(p), dispatch(d) {}

  void output_pole_tables(
    early_bindings_type const& decl
  );
  void output_dispatch_table(
    const early_bindings_type& decl
  );

  void rerank(
    const early_bindings_type& decl
  );

private:
  void rerank_unary();
  void rerank_other();
};

void output_device::rerank(
  const early_bindings_type& decl
) {
  if(decl.arity == 1)
    rerank_unary();
  
  else
    rerank_other();
}

void output_device::rerank_unary() {
  foreach(pole_table_t::const_reference h, poles) {
    foreach(klass_t const* k, h) {
      dispatch_t::mapped_type const& target = dispatch.at( signature_t::unary(k) );
      invoker_t ptr = target.second;

      uintptr_t value = reinterpret_cast<uintptr_t>(ptr ? ptr : output.ambiguity_policy.bad_dispatch);
      ht.insert(std::make_pair(k, value));
    }
  }
}

void output_device::rerank_other() {
  std::size_t incr = 1;

  foreach(pole_table_t::const_reference h, poles) {
    std::size_t current = 0;

    foreach(klass_t const* k, h) {
      // insert expects 2-aligned values
      ht.insert(std::make_pair(k, 2 * current));
      current += incr;
    }

    incr = current;
  }

  max_index = incr;
}

namespace {

struct rankhash_adder {
  hash_table_type const& ht;

  std::size_t operator()(std::size_t acc, klass_t const* k)
  { return acc + ht.at(k); }
};

} // namespace <>

static void
make_assignment(
  signature_t const& sig
, invoker_t  inv
, invoker_t* table
, hash_table_type const& ht
) {
  BOOST_ASSERT(inv);

  rankhash_adder adder = { ht };

  std::size_t const index = std::accumulate(
    sig.array().begin(), sig.array().end(), 0,
    adder
  );

  table[index/2] = inv;
}

void output_device::output_dispatch_table(
  const early_bindings_type& decl
) {
  if(decl.arity == 1)
    return;

  invoker_t* const table = new invoker_t [ max_index ];
  output.table = table;

  std::fill_n(table, max_index, output.ambiguity_policy.bad_dispatch);

  // assign dispatch table
  foreach(dispatch_t::const_reference p, dispatch) {
    invoker_t inv = p.second.second;
    if(inv)
      make_assignment(p.first, inv, table, ht);
  }
}

static void fill_map(
  poles_map_type& a
, pole_table_t::const_reference t
, hash_table_type const& ht
) {
  /// split \c t between static and dynamic id
  std::vector<klass_t const*> dynamics ( boost::begin(t), boost::end(t) );

  a.create( dynamics.size() );

  // insert expects 2-aligned values
  foreach(klass_t const* k, dynamics) {
    rtti_type key   = k->get_id();
    uintptr_t value = ht.at(k);
    a.insert(key, value);
    BOOST_ASSERT( (value & 1) == 0 );
  }
}

void output_device::output_pole_tables(
  early_bindings_type const& decl
) {
  std::size_t const arity = decl.arity;

  for(std::size_t i = 0; i < arity; ++i) {
    pole_table_t::const_reference t = poles[i];
    poles_map_type& pm = *output.poles[i];

    fill_map(pm, t, ht);
  }
}

void rtti_dispatch::output_tables(
  seal_table_type& f,
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const early_bindings_type& decl
) {
  output_device dev ( f, pole_table, dispatch );

  dev.rerank(decl);
  dev.output_dispatch_table(decl);
  dev.output_pole_tables(decl);
}
