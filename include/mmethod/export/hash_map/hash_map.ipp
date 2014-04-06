//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_BASE_IPP
#define RTTI_MMETHOD_HASH_MAP_BASE_IPP

#include "mmethod/config.hpp"
#include "mmethod/export/hash_map/hash_map.hpp"
#include "mmethod/export/hash_map/bucket.ipp"

#include "mmethod/rtti/holder/node.hpp"

#include <boost/type_traits/alignment_of.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace hash {
namespace detail {

inline hash_map::hash_map()
: m_mask(0), m_logsz(0) {}

inline index_type         hash_map::hash(key_type a)   const BOOST_NOEXCEPT_OR_NOTHROW {
  uintptr_t v = uintptr_t(a);
  v /= boost::alignment_of<rtti::rtti_node>::value;
  return index_type( v & m_mask );
}

inline hash_map::iterator hash_map::zero()             const BOOST_NOEXCEPT_OR_NOTHROW { return &m_array[0]; }
inline hash_map::iterator hash_map::find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW {
  bucket_t* bucket = &m_array[ hash(key) ];

  if(BOOST_LIKELY( bucket->key() == key ))
    return bucket;

  return do_find(key);
}

#ifdef MMETHOD_INLINE_DO_FIND
inline hash_map::iterator hash_map::do_find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW {
  std::size_t const index = hash(key);
  bucket_t* ptr = &m_array[index];

  // empty bucket sentinel is last of m_array -> forces stop since empty
  while(
      (! ptr->empty())
   && (ptr->key() != key)
  ) {
    ++ptr;
  }

  // assert empty() => (key() != key)
  BOOST_ASSERT( !ptr->empty() || ptr->key() != key );

  return ptr;
}
#endif

}}} // namespace rtti::mmethod::detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif