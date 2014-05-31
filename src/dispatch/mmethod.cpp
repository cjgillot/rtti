//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include "foreach.hpp"

#include <vector>

void rtti_dispatch::process_declaration(early_bindings_type const& decl, seal_table_type& output)
{
  std::size_t arity = decl.arity;

  /// hierarchy stuff
  std::vector<std::vector<rtti_hierarchy> > hierarchies ( arity );
  foreach(binding_type const& over, decl.vector) {
    signature_type const& s = over.first;
    for(std::size_t i = 0; i < arity; ++i)
      hierarchies[i].push_back( s[i] );
  }

  /// order poles
  pole_table_t pole_table ( arity );
  order_poles(pole_table, hierarchies);

  /// overload list
  overloads_t overloads; overloads.reserve( decl.vector.size() );
  foreach(binding_type const& over, decl.vector) {
    signature_type const& h = over.first;

    overloads.push_back( overload_t(make_signature(h, pole_table), over.second) );
  }

  /// fill up dispatch table
  dispatch_t dispatch_table;
  dispatch(dispatch_table, overloads, pole_table, output.ambiguity_policy.ahndl);

  /// output
  output_tables(output, pole_table, dispatch_table, decl);
}
