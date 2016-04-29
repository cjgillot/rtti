//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "early.hpp"

void
rtti::mmethod::detail::init_table(
  std::size_t arity
, early_bindings_type& eb
) {
  eb = new early_bindings_struct();
  eb->arity = arity;
}

void
rtti::mmethod::detail::inse_table(
  std::size_t arity
, early_bindings_type& eb
, invoker_t inv
, rtti_hierarchy* hiers
, duplicator* d
) {
  BOOST_ASSERT(eb);

  // this function is called at global initialization
  // no synchronization should be required
  // but seal_table is not protected

  d->load(arity, hiers);
  do {
    // signature_type is a vector -> copies the array hiers
    rtti_signature sig ( hiers, hiers+arity );
    eb->vector.push_back(std::make_pair(sig, inv));
  }
  while(d->next());
}

void
rtti::mmethod::detail::seal_table(
  std::size_t /*arity*/
, early_bindings_type& eb
, seal_table_type& seal
) {
  // this function is called exactly once for each [seal]
  // we assume this function is called strictly after [inse_table]
  // finished on the same [eb]
  rtti_dispatch::process_declaration(*eb, seal);
  delete eb; eb = NULL;
}
