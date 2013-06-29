#include "rtti/hash/hash_map.hpp"

#include "util/assert.hpp"

using rtti::rtti_type;

using rtti::detail::rtti_node;
using rtti::detail::rtti_hierarchy;

using rtti::hash::detail::bucket_t;
using rtti::hash::detail::hash_map;
using rtti::hash::detail::value_type;

// can be moved as non-member
value_type
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_NONNULL(3) ATTRIBUTE_HOT()
rtti::hash::detail::do_fetch_pole(
  hash_map const& map
, rtti_hierarchy rt0
, hash_map::iterator it0
) noexcept {
  const rtti_type id0 = rt0->id;

  for(rtti_node const* rt = rt0->base; rt; rt = rt->base) {
    hash_map::iterator it = map.find(rt->id);

    if(LIKELY( !it->empty() )) {
#if MMETHOD_USE_THREAD
      util::stw_lock::convert_guard guard { m_mutex };
#endif

      const_cast<hash_map&>(map).insert_at( it0, id0, it->value() );
#if MMETHOD_USE_DEEP_CACHE
      for(rtti_node const* rt2 = rt0->base; rt2 != rt; rt2 = rt2->base)
        const_cast<hash_map&>(map).insert( rt2->id, it->value() );
#endif

      return it->value();
    }
  }

  return map.zero()->value();
}
