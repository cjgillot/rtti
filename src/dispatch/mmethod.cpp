#include <vector>

#include "rtti/hash/hash.hpp"

#include "hierarchy.hpp"
#include "overloads.hpp"

#include "early.hpp"

#include <boost/foreach.hpp>

// from dispatch.cpp
extern void dispatch(
  dispatch_t &dispatch,
  overloads_t& overloads,
  const pole_table_t &pole_table
);

// from gen_mph.cpp/gperf.cc
extern void gen_mph(
  seal_table_type& f,
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const early_bindings_type& decl
);

// from order_poles.cpp
extern void order_poles(
  pole_table_t& pole_table,
  std::vector<hierarchy_t>& hierarchies
);
extern void print_poles(
  seal_table_type&,
  early_bindings_type const& decl,
  pole_table_t const& pole_table
);
extern void print_initializer(
  seal_table_type& ofile
, early_bindings_type const& decl
);

void process_declaration(early_bindings_type const& decl, seal_table_type& output)
{
  std::size_t arity = decl.arity;

  /// hierarchy stuff
  std::vector<hierarchy_t> hierarchies ( arity );

  /// parsing input
  overloads_t overloads; overloads.reserve( decl.vector.size() );
  BOOST_FOREACH(binding_type const& over, decl.vector) {
    signature_type const& h = over.first;

    overloads.push_back( std::make_pair(make_signature(h, hierarchies), over.second) );
  }

  /// shrink hierarchies
  BOOST_FOREACH(hierarchy_t& hier, hierarchies)
    hier.shrink();

  /// order poles
  pole_table_t pole_table ( arity );
  order_poles(pole_table, hierarchies);

  /// fill up dispatch table
  dispatch_t dispatch_table;
  dispatch(dispatch_table, overloads, pole_table);

  /// prepare poles for output : link each pole to a signature in which it appears
  /// \warning This code must be after any change to \c overloads
  for(const overload_t& sig : overloads)
    for(const klass_t* k : sig.first.array())
      const_cast<signature_t const*&>( k->sig ) = &sig.first;

  /// create perfect hash function
  gen_mph(output, pole_table, dispatch_table, decl);

  /// print pole tables
  print_poles(output, decl, pole_table);
}
