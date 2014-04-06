//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "early.hpp"

void
boost::mmethod::detail::init_table(
  std::size_t arity
, invoker_table_type& tbl
) {
  early_bindings_type* eb = new early_bindings_type();
  eb->arity = arity;

  tbl = reinterpret_cast<invoker_table_type>( eb );
}

void
boost::mmethod::detail::inse_table(
  std::size_t arity
, invoker_table_type& table
, invoker_t inv
, rtti_hierarchy* hiers
) {
  BOOST_ASSERT(table);

  early_bindings_type* eb = reinterpret_cast<early_bindings_type*>(table);

  // this function is called at global initialization
  // no synchronization should be required
  // but seal_table is not protected

  eb->vector.push_back(std::make_pair(signature_type(hiers, hiers+arity), inv));
}

void
boost::mmethod::detail::seal_table(
  std::size_t /*arity*/
, invoker_table_type table
, seal_table_type& seal
) {
  // this function is called exactly once for each [seal]
  // we assume this function is called strictly after [inse_table] finished on the same [table]
  early_bindings_type* eb = reinterpret_cast<early_bindings_type*>(table);

  boost_mmethod_dispatch::process_declaration(*eb, seal);

  delete eb;
}
