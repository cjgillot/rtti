#include "mmethod/dynamic/poles.hpp"
#include "mmethod/dynamic/dynamic.hpp"

#include "util/stw_lock.hpp"
#include "util/attribute.hpp"

#include <type_traits>
#include <boost/functional/hash/hash.hpp>

using namespace rtti;
using namespace rtti::dmethod;
using namespace rtti::dmethod::detail;

using rtti::dmethod::detail::functor_t;
using rtti::dmethod::detail::invoker_entry;
using rtti::dmethod::detail::poles_map_type;
using rtti::dmethod::detail::invoker_table_type;

using rtti::hash::detail::value_type;

constexpr value_type INIT_VALUE = static_cast<value_type>( 0ul );

void
rtti::dmethod::detail::init_pole_unary(poles_map_type& map) {
  map.mem.create<1>();
  map.mem.insert( rtti_type(0ul), INIT_VALUE );

  map.poles.create<1>();
  map.poles.insert( rtti_type(0ul), INIT_VALUE );

  ASSERT( map.smallint == 0 );
}

void
rtti::dmethod::detail::insert_pole_unary(
  poles_map_type& map
, rtti_node const* hier
, functor_t inv
) {
#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { map.lock };
#endif

  auto it = map.poles.find( hier->id );

  value_type ret = static_cast<value_type>( (std::uintptr_t)inv );
  if( it->value() == ret )
    // existing pole, nothing to do
    return;

  map.poles.insert_at( it, hier->id, ret );
  map.mem.flush( map.poles );
}

functor_t
rtti::dmethod::detail::retract_pole_unary(
  poles_map_type& map
, rtti_node const* hier
) {
#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { map.lock };
#endif

  auto it = map.poles.find( hier->id );

  if( it->empty() )
    // non-existing pole, nothing to do
    return 0;

  std::uintptr_t ret = (std::uintptr_t)it->value();
  map.poles.erase( it );
  map.mem.flush( map.poles );
  return reinterpret_cast<functor_t>(ret);
}
