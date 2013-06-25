#ifndef RTTI_MMETHOD_HASH_MAP_IPP
#define RTTI_MMETHOD_HASH_MAP_IPP

#include "rtti/hash/hash_map.hpp"

namespace rtti {
namespace hash {
namespace detail {

inline constexpr hash_map_base::hash_map_base() {}
inline constexpr hash_map::hash_map() {}

namespace {
inline constexpr std::size_t
nbits(std::size_t n) {
  return (n == 0)
    ? 0
    : 1 + nbits(n >> 1)
    ;
}
} // namespace <>

template<std::size_t N>
inline void hash_map::create() {
  constexpr std::size_t logsz = 1 + nbits(N | 7);
  m_base.create(logsz);
}

#ifndef MMETHOD_IN_MMETHOD
// use by fetch_pole

inline index_type ATTRIBUTE_PURE
hash_map_base::hash(key_type a) const noexcept
{ return index_type{ std::uintptr_t(a) & m_mask }; }

inline hash_map_base::iterator ATTRIBUTE_PURE hash_map_base::zero() const noexcept { return &m_array[0]; }
inline hash_map     ::iterator ATTRIBUTE_PURE hash_map     ::zero() const noexcept { return m_base.zero(); }

inline hash_map_base::iterator ATTRIBUTE_PURE hash_map_base::find(key_type key) const noexcept {
  bucket_t* bucket = &m_array[ hash(key) ];

  if(LIKELY( bucket->key == key ))
    return bucket;

  return do_find(key);
}
inline hash_map     ::iterator ATTRIBUTE_PURE hash_map     ::find(key_type key) const noexcept {
  return m_base.find(key);
}

inline std::uintptr_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
hash_map::fetch_pole(
  const ::rtti::detail::rtti_node* rt
) const noexcept {
  const rtti_type id0 = rt->id;
#if MMETHOD_USE_THREAD
  util::stw_lock::fetch_guard guard { m_mutex };
#endif

  iterator it0;

#if MMETHOD_USE_INLINE_FIND
  {
    it0 = &m_base.m_array[ m_base.hash(id0) ];

#if MMETHOD_USE_INLINE_DO_FIND
    do {
      if(LIKELY( it0->key == id0 ))
        return it0->value;

      ++it0;
    }
    while(UNLIKELY( !it0->empty() ));
#else  // MMETHOD_USE_INLINE_DO_FIND
    if(LIKELY( it0->key == id0 ))
      return it0->value;

    it0 = m_base.do_find(id0);
#endif // MMETHOD_USE_INLINE_DO_FIND
  }
#else  // MMETHOD_USE_INLINE_FIND
  t0 = find(id0);
#endif // MMETHOD_USE_INLINE_FIND

  if(LIKELY( !it0->empty() ))
    return it0->value;

  return do_fetch_pole(rt, it0);
}

#endif

}}} // namespace rtti::mmethod::detail

#endif
