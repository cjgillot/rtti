//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_BASE_TPP
#define RTTI_MMETHOD_HASH_MAP_BASE_TPP

#include "mmethod/hash/hash_map/hash_map_base.hpp"
#include "mmethod/hash/hash_map/hash_map_base.ipp"

namespace rtti {
namespace hash {
namespace detail {

inline index_type              ATTRIBUTE_PURE hash_map_base::hash(key_type a)   const BOOST_NOEXCEPT_OR_NOTHROW { return index_type( uintptr_t(a) & m_mask ); }

inline hash_map_base::iterator ATTRIBUTE_PURE hash_map_base::zero()             const BOOST_NOEXCEPT_OR_NOTHROW { return &m_array[0]; }
inline hash_map_base::iterator ATTRIBUTE_PURE hash_map_base::find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW {
  bucket_t* bucket = &m_array[ hash(key) ];

  if(LIKELY( bucket->key() == key ))
    return bucket;

  return do_find(key);
}

}}} // namespace rtti::mmethod::detail

#endif
