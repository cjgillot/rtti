//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include <vector>
#include <boost/foreach.hpp>

void rtti_dispatch::process_declaration(early_bindings_type const& decl, seal_table_type& output)
{
  std::size_t arity = decl.arity;

  /// hierarchy stuff
  std::vector<hierarchy_t> hierarchies ( arity );

  /// parsing input
  overloads_t overloads; overloads.reserve( decl.vector.size() );
  BOOST_FOREACH(binding_type const& over, decl.vector) {
    signature_type const& h = over.first;

    overloads.push_back( overload_t(make_signature(h, hierarchies), over.second) );
  }

  /// order poles
  pole_table_t pole_table ( arity );
  order_poles(pole_table, hierarchies);

  /// fill up dispatch table
  dispatch_t dispatch_table;
  dispatch(dispatch_table, overloads, pole_table);

  /// prepare poles for output : link each pole to a signature in which it appears
  /// \warning This code must be after any change to \c overloads
  BOOST_FOREACH(const overload_t& sig, overloads)
    BOOST_FOREACH(const klass_t* k, sig.first.array())
      const_cast<signature_t const*&>( k->sig ) = &sig.first;

  /// output
  output_tables(output, pole_table, dispatch_table, decl);
}
