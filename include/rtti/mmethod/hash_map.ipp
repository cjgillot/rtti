#ifndef RTTI_MMETHOD_HASH_MAP_IPP
#define RTTI_MMETHOD_HASH_MAP_IPP

#include "rtti/mmethod/hash_map.hpp"

namespace rtti {
namespace mmethod {
namespace hash_map_detail {

inline constexpr hash_map_base::hash_map_base() {}

#ifdef MMETHOD_IN_MMETHOD
// use by generated code

namespace {
inline constexpr std::size_t
nbits(std::size_t n) {
  return (n == 0)
    ? 0
    : 1 + nbits(n >> 1)
    ;
}
} // namespace <>

#ifdef MMETHOD_USE_SMALLARRAY
template<std::size_t N>
inline constexpr hash_map::hash_map(bucket_t(&small)[N])
: m_smallcount(N)
, m_smallarray(small)
{
  static_assert( N > 0, "mmethod : empty array" );
}
#else
inline constexpr hash_map::hash_map() {}
#endif

template<std::size_t N>
inline void hash_map::create() {
  constexpr std::size_t logsz = 1 + nbits(N | 7);
  do_create(logsz);
}

#else
// use by fetch_pole

inline index_type
hash_map_base::hash(key_type a) const
{ return index_type{ a & m_mask }; }

inline hash_map_base::iterator hash_map_base::find(key_type key) const {
  index_type index = hash(key);

  bucket_t* bucket = &m_array[index];

  if(LIKELY( bucket->key == key ))
    return bucket;

  return do_find(index, key);
}

inline hash_map::iterator hash_map::find(key_type key) const {
#ifdef MMETHOD_USE_SMALLARRAY
  if(LIKELY( key < m_smallcount ))
    return m_smallarray + key;
#endif

  return hash_map_base::find(key);
}

inline std::uintptr_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
fetch_pole(
  poles_map_type& map
, const ::rtti::detail::rtti_node* rt
) noexcept {
  const rtti_type id0 = rt->id;
  const hash_map::iterator it0 = map.find(id0);

  if(LIKELY( !it0->empty() ))
    return it0->value;

  return map.do_fetch_pole(rt, it0);
}

#endif

}}} // namespace rtti::mmethod::detail

#endif
