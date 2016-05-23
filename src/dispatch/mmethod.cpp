//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"
#include "hierarchy.hpp"
#include "signature.hpp"

#include "foreach.hpp"

#include <vector>

void rtti_dispatch::process_declaration(early_bindings_struct const& decl,
                                        seal_table_type& output)
{
  std::size_t const arity = decl.arity;

  /// hierarchy stuff
  std::vector<std::vector<rtti_hierarchy> > hierarchies ( arity );
  foreach(rtti_binding const& over, decl.vector) {
    rtti_signature const& s = over.first;
    for(std::size_t i = 0; i < arity; ++i) {
      hierarchies[i].push_back( s[i] );
    }
  }

  /// order poles
  pole_table_t pole_table ( arity );
  order_poles(pole_table, hierarchies);

  /// fill up dispatch table
  dispatch_t dispatch_table; {
    // declared overloads
    foreach(rtti_binding const& over, decl.vector) {
      rtti_signature const& h = over.first;
      signature_t sig = make_signature(h, pole_table);

      if(!over.second) {
        continue;
      }

      invoker_t& disp = dispatch_table[sig];
      if(!disp) {
        disp = over.second;
        continue;
      }

      if(disp != over.second) {
        // don't know what to do -> die
        std::ostringstream msg;
        msg << "Duplicate overload detected in mmethod, aborting !" << std::endl;
        msg << "Competing overloads: "
            << (void*)disp << " and " << (void*)over.second << std::endl;
        msg << "for type tuple: ( ";
        foreach(rtti_hierarchy h, over.first)
          msg << h << " ";
        msg << std::endl;

        BOOST_THROW_EXCEPTION(std::runtime_error(msg.str()));
      }
    }
  }
  dispatch(dispatch_table, pole_table, output.ambiguity_policy.ahndl);

  /// output
  output_tables(output, pole_table, dispatch_table, decl);
}
