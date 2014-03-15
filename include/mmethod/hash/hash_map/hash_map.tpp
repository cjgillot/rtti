//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_BASE_TPP
#define RTTI_MMETHOD_HASH_MAP_BASE_TPP

#include "mmethod/hash/hash_map/hash_map.hpp"
#include "mmethod/hash/hash_map/hash_map.ipp"

#include "rtti/holder/node.hpp"
#include <boost/type_traits/alignment_of.hpp>

namespace rtti {
namespace hash {
namespace detail {

inline index_type         ATTRIBUTE_PURE hash_map::hash(key_type a)   const BOOST_NOEXCEPT_OR_NOTHROW {
  uintptr_t v = uintptr_t(a);
  v /= boost::alignment_of<rtti::rtti_node>::value;
  return index_type( v & m_mask );
}

inline hash_map::iterator ATTRIBUTE_PURE hash_map::zero()             const BOOST_NOEXCEPT_OR_NOTHROW { return &m_array[0]; }
inline hash_map::iterator ATTRIBUTE_PURE hash_map::find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW {
  bucket_t* bucket = &m_array[ hash(key) ];

  if(LIKELY( bucket->key() == key ))
    return bucket;

  return do_find(key);
}

}}} // namespace rtti::mmethod::detail

#endif
