#include "rtti/mmethod/hash_map.hpp"
#include "rtti/hash.hpp"

#include "util/assert.hpp"

#include <algorithm>
#include <functional>

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::mmethod::hash_map_detail::bucket_t;
using rtti::mmethod::hash_map_detail::hash_map;

#ifndef __EXCEPTIONS__
#define try if(true)
#define catch(...) else if(false)
#endif

/// bucket_t implementation
//@{
inline void bucket_t::reset() { value = 0; }
inline void bucket_t::set(rtti_type k, std::uintptr_t v) {
  key = k;
  value = v;
  ASSERT( !empty() );
}
//@}

/// hash_map implementation
//@{
hash_map::~hash_map() {}

inline hash_map::iterator hash_map::zero() const {
#if MMETHOD_USE_SMALLARRAY
  return &m_smallarray[0];
#else
  return &m_array[0];
#endif
}

inline void hash_map::insert_at(iterator it, key_type key, value_type value) {
  return hash_map_base::insert_at(it, key, value);
}

void hash_map::insert(rtti_type key, std::uintptr_t value) noexcept {
#ifdef MMETHOD_USE_SMALLARRAY
  if( key < m_smallcount ) {
    m_smallarray[key].set(key, value);
    return;
  }
#endif

  return hash_map_base::insert(key, value);
}
//@}

std::uintptr_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
hash_map::do_fetch_pole(
  const rtti_node* rt
, hash_map::iterator it0
) noexcept {
  const rtti_type id0 = rt->id;

  while((rt = rt->base)) {
    iterator it = find(rt->id);

    if(LIKELY( !it->empty() )) {
#ifdef MMETHOD_USE_THREAD
      std::lock_guard<std::mutex> guard { m_mutex };
#endif
      insert_at( it0, id0, it->value );
      return it->value;
    }
  }

  return zero()->value;
}
