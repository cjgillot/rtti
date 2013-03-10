#include "rtti/hash/hash_map.hpp"

#include "util/assert.hpp"

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::hash::detail::bucket_t;
using rtti::hash::detail::hash_map;

#ifndef __EXCEPTIONS__
#define try if(true)
#define catch(...) else if(false)
#endif

/// bucket_t implementation
//@{
inline void bucket_t::reset() { value = 1; }
inline void bucket_t::set(rtti_type k, std::uintptr_t v) {
  key = k;
  value = v;
  ASSERT( !empty() );
}
//@}

/// hash_map implementation
//@{
hash_map::~hash_map() {}

void hash_map::flush(hash_map const& m) noexcept {
  m_base.import(m.m_base);
}

void hash_map::insert_at(iterator it, key_type key, value_type value) {
  return m_base.insert_at(it, key, value);
}

void hash_map::insert(rtti_type key, std::uintptr_t value) {
#if MMETHOD_USE_SMALLARRAY
  if( key < m_smallcount ) {
    m_smallarray[key].set(key, value);
    return;
  }
#endif

  m_base.insert(key, value);
}

void hash_map::erase(iterator it) {
#if MMETHOD_USE_SMALLARRAY
  if( m_smallarray <= it && it < m_smallarray + m_smallcount ) {
    it->reset();
    return;
  }
#endif

  m_base.erase(it);
}
//@}

std::uintptr_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
hash_map::do_fetch_pole(
  const rtti_node* rt0
, hash_map::iterator it0
) const noexcept {
  const rtti_type id0 = rt0->id;

  for(rtti_node const* rt = rt0->base; rt; rt = rt->base) {
    iterator it = find(rt->id);

    if(LIKELY( !it->empty() )) {
#if MMETHOD_USE_THREAD
      util::stw_lock::convert_guard guard { m_mutex };
#endif

      const_cast<hash_map*>(this)->insert_at( it0, id0, it->value );
      for(rtti_node const* rt2 = rt0->base; rt2 != rt; rt2 = rt2->base)
        const_cast<hash_map*>(this)->insert( rt2->id, it->value );

      return it->value;
    }
  }

  return zero()->value;
}
