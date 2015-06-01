//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include "foreach.hpp"

#include <numeric>

using rtti::hash::detail::value_type;

typedef boost::unordered_map<klass_t const*, value_type> hash_table_type;

class output_device {
  seal_table_type& output;

  hash_table_type ht;
  std::size_t max_index;
  value_type fallback;

  pole_table_t const& poles;
  dispatch_t   const& dispatch;

public:
  output_device(seal_table_type& o, pole_table_t const& p, dispatch_t const& d)
  : output(o), poles(p), dispatch(d) {}

  void output_pole_tables(
    early_bindings_struct const& decl
  );
  void output_dispatch_table(
    early_bindings_struct const& decl
  );

  void rerank(
    early_bindings_struct const& decl
  );

private:
  void rerank_unary();
  void rerank_other();
};

void output_device::rerank(
  const early_bindings_struct& decl
) {
  if(decl.arity == 1) {
    fallback = reinterpret_cast<value_type>(output.ambiguity_policy.bad_dispatch);
    rerank_unary();
  }
  else {
    fallback = 0;
    rerank_other();
  }
}

void output_device::rerank_unary() {
  foreach(pole_table_t::const_reference h, poles) {
    foreach(klass_t const* k, h.range()) {
      dispatch_t::mapped_type const& target = dispatch.at( signature_t::unary(k) );
      invoker_t ptr = target.second;
      value_type value = reinterpret_cast<value_type>(ptr);
      if(!value) value = fallback;

      ht.insert(std::make_pair(k, value));
    }
  }
}

void output_device::rerank_other() {
  std::size_t incr = 1;

  foreach(pole_table_t::const_reference h, poles) {
    // index zero is used for fallback case
    std::size_t current = 1;

    foreach(klass_t const* k, h.range()) {
      // insert expects 2-aligned values
      BOOST_ASSERT(current != fallback);
      ht.insert(std::make_pair(k, current));
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
    sig.array().begin(), sig.array().end(), 0ul,
    adder
  );

  table[index] = inv;
}

void output_device::output_dispatch_table(
  const early_bindings_struct& decl
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
  hierarchy_t::range_type const& r = t.range();
  std::vector<klass_t const*> dynamics ( boost::begin(r), boost::end(r) );

  a.create( dynamics.size() );

  // insert expects 2-aligned keys - ensured by node.hpp
  foreach(klass_t const* k, dynamics) {
    rtti_type key   = k->get_id();
    uintptr_t value = ht.at(k);

    a.insert(key, value);
  }
}

void output_device::output_pole_tables(
  early_bindings_struct const& decl
) {
  std::size_t const arity = decl.arity;

  for(std::size_t i = 0; i < arity; ++i) {
    pole_table_t::const_reference t = poles[i];
    poles_map_type& pm = *output.poles[i];

    pm.set_fallback(fallback);
    fill_map(pm, t, ht);
  }
}

void rtti_dispatch::output_tables(
  seal_table_type& f,
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const early_bindings_struct& decl
) {
  output_device dev ( f, pole_table, dispatch );

  dev.rerank(decl);
  dev.output_dispatch_table(decl);
  dev.output_pole_tables(decl);
}
