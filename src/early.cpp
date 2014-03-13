#include "early.hpp"

void
rtti::dmethod::detail::init_table(
  std::size_t arity
, invoker_table_type& tbl
) {
  early_bindings_type* eb = new early_bindings_type();
  eb->arity = arity;

  tbl = reinterpret_cast<invoker_table_type>( eb );
}

void
rtti::dmethod::detail::inse_table(
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

extern void process_declaration(early_bindings_type const& decl, seal_table_type& output);

void
rtti::dmethod::detail::seal_table(
  std::size_t /*arity*/
, invoker_table_type table
, seal_table_type& seal
) {
  // this function is called exactly once for each [seal]
  // we assume this function is called strictly after [inse_table] finished on the same [table]
  early_bindings_type* eb = reinterpret_cast<early_bindings_type*>(table);

  process_declaration(*eb, seal);

  delete eb;
}
