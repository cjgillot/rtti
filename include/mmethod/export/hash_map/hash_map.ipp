//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_HASH_MAP_BASE_IPP
#define BOOST_MMETHOD_HASH_MAP_BASE_IPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/export/hash_map/hash_map.hpp"
#include "boost/mmethod/export/hash_map/bucket.ipp"

#include "boost/mmethod/rtti/holder/node.hpp"

#include <boost/type_traits/alignment_of.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace mmethod {
namespace hash_detail {

#ifndef BOOST_NO_CXX11_SMART_PTR
BOOST_CONSTEXPR
#endif
inline hash_map::hash_map() BOOST_NOEXCEPT_OR_NOTHROW
: m_mask(0), m_logsz(0), m_fallback(0) {}

inline index_type         hash_map::hash(key_type a)   const BOOST_NOEXCEPT_OR_NOTHROW {
  uintptr_t v = uintptr_t(a);
  v /= boost::mmethod::rtti_node::alignment::value;
  return index_type( v & m_mask );
}

inline hash_map::iterator hash_map::find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW {
  bucket_t* bucket = &m_array[ hash(key) ];

  if(BOOST_LIKELY( bucket->key() == key ))
    return bucket;

  return do_find(key);
}

#ifdef BOOST_MMETHOD_INLINE_DO_FIND
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

}}} // namespace boost::mmethod::hash_detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
