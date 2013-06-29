#ifndef RTTI_MMETHOD_HASH_MAP_BASE_IPP
#define RTTI_MMETHOD_HASH_MAP_BASE_IPP

#include "rtti/hash/hash_map_base.hpp"

namespace rtti {
namespace hash {
namespace detail {

inline index_type              ATTRIBUTE_PURE hash_map_base::hash(key_type a)   const noexcept { return index_type{ std::uintptr_t(a) & m_mask }; }

inline hash_map_base::iterator ATTRIBUTE_PURE hash_map_base::zero()             const noexcept { return &m_array[0]; }
inline hash_map_base::iterator ATTRIBUTE_PURE hash_map_base::find(key_type key) const noexcept {
  bucket_t* bucket = &m_array[ hash(key) ];

  if(LIKELY( bucket->key() == key ))
    return bucket;

  return do_find(key);
}

}}} // namespace rtti::mmethod::detail

#endif
