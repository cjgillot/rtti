#include "mmethod/hash/fetch_pole.hpp"

#include <boost/assert.hpp>

using rtti::rtti_type;

using rtti::detail::rtti_node;
using rtti::detail::rtti_hierarchy;

using rtti::hash::detail::bucket_t;
using rtti::hash::detail::hash_map;
using rtti::hash::detail::value_type;

// can be moved as non-member
value_type
ATTRIBUTE_NONNULL(2) ATTRIBUTE_NONNULL(3)
rtti::hash::detail::do_fetch_pole(
  hash_map const& map
, rtti_hierarchy rt0
, hash_map::iterator it0
) BOOST_NOEXCEPT_OR_NOTHROW {

  const rtti_type id0 = rt0->id;

  rtti_node const* rt = rt0;

  for(;;) {
    // exit condition
    if(rt->arity == 0)
      break;

    // by definition of the poles,
    // a fork either is a pole, or has all its branches behave the same way
    rt = rt0->base[0];

    BOOST_ASSERT(rt);

    // common case
    hash_map::iterator it = map.find(rt->id);

    if(LIKELY( !it->empty() )) {

      const_cast<hash_map&>(map).insert_at( it0, id0, it->value() );
#if MMETHOD_USE_DEEP_CACHE
      for(rtti_node const* rt2 = rt0->base[0]; rt2 != rt; rt2 = rt2->base[0])
        const_cast<hash_map&>(map).insert( rt2->id, it->value() );
#endif

      return it->value();
    }
  }

  return map.zero()->value();
}
