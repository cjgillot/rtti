#ifndef RTTI_MMETHOD_HASH_FETCH_POLE_HPP
#define RTTI_MMETHOD_HASH_FETCH_POLE_HPP

#include "mmethod/hash/hash_map/hash_map.hpp"
#include "mmethod/hash/hash_map/hash_map.ipp"

namespace rtti {
namespace hash {
namespace detail {

// can be moved as non-member
extern value_type
ATTRIBUTE_NONNULL(2) ATTRIBUTE_NONNULL(3)
do_fetch_pole(
  hash_map const&
, rtti_hierarchy rt0
, hash_map::iterator it0
) BOOST_NOEXCEPT_OR_NOTHROW;

inline value_type
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
fetch_pole(
  hash_map const& map
, rtti_hierarchy rt
) BOOST_NOEXCEPT_OR_NOTHROW {

  const rtti_type id0 = rtti_get_id(rt);
  hash_map::iterator it0;

  {
    it0 = map.find(id0);

    if(LIKELY( !it0->empty() ))
      return it0->value();
  }

  return do_fetch_pole(map, rt, it0);
}

}}} // namespace rtti::hash::detail

#endif
