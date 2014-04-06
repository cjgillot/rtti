//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/export/hash_map.hpp"

using boost::mmethod::rtti_type;
using boost::mmethod::detail::rtti_node;
using boost::mmethod::hash_detail::bucket_t;
using boost::mmethod::hash_detail::hash_map;
using boost::mmethod::hash_detail::key_type;
using boost::mmethod::hash_detail::value_type;

#define BADBUCKET (&(m_array[m_mask + 1]))

/// bucket_t implementation
//@{
inline void bucket_t::reset() BOOST_NOEXCEPT_OR_NOTHROW { m_value = 1; }
inline void bucket_t::set(key_type k, value_type v) BOOST_NOEXCEPT_OR_NOTHROW {
  m_value = static_cast<storage_type>(v);
  m_key = k;
  BOOST_ASSERT( !empty() );
}
//@}

/// hash_map implementation
//@{
void hash_map::move(hash_map& o) BOOST_NOEXCEPT_OR_NOTHROW {
  BOOST_ASSERT( this != &o );

  m_mask = o.m_mask;
  m_logsz= o.m_logsz;

  m_array.swap(o.m_array);
}

void hash_map::flush(hash_map const& o) {
  BOOST_ASSERT( this != &o );

  m_array.reset();

  create(1 + o.m_logsz);

  std::size_t sz = 1 << o.m_logsz;
  for(std::size_t i = 0; i < sz; ++i)
    if( !o.m_array[i].empty() )
      insert(o.m_array[i].key(), o.m_array[i].value());
}
//@}

namespace {
// helper function for [do_find] and [insert]
inline bucket_t*
probe_table(bucket_t* const m_array, std::size_t index, rtti_type key) BOOST_NOEXCEPT_OR_NOTHROW {
  // use linear probing
  bucket_t* ptr = &m_array[index];

  // empty bucket sentinel is last of m_array -> forces stop since empty
  while(
      (! ptr->empty())
   && (ptr->key() != key)
  ) {
    ++ptr;
  }

  return ptr;
}
} // namespace <anonymous>

#ifndef MMETHOD_INLINE_DO_FIND

/// hash_map find()
//@{
hash_map::iterator
hash_map::do_find(rtti_type key) const BOOST_NOEXCEPT_OR_NOTHROW {
  return probe_table(
    m_array.get(), hash(key), key
  );
}
//@}

#endif

/// hash_map insert()
//@{
// [it] has been returned by [find(key, value)]
void hash_map::insert_at(iterator it, key_type key, value_type value) {
  if( it->empty() && it != BADBUCKET )
    return const_cast<bucket_t*>(it)->set(key, value);

  insert(key, value);
}

void hash_map::insert(key_type key, value_type value) {
  index_type index = hash(key);

  bucket_t* bucket = probe_table(
    m_array.get(), index, key
  );
  if(bucket != BADBUCKET)
    return bucket->set(key, value);
  
  insert_need_resize(key, value);
}

void hash_map::erase(iterator iter) {
  bucket_t* it = const_cast<bucket_t*>(iter);
  it->reset();
  ++it;

  for(; !it->empty(); ++it) {
    key_type   key = it->key();
    value_type val = it->value();

    it->reset();
    insert(key, val);
  }
}
//@}

/// hash_map resize()
//@{
void hash_map::insert_need_resize(key_type key, value_type value) {
  hash_map repl;
  repl.flush(*this);
  repl.insert(key, value);
  move(repl);
}
//@}

inline static std::size_t
nbits(std::size_t n) {
  std::size_t ret = 0;
  for(; n; n>>=1)
    ++ret;
  return ret;
}

/// hash_map create()
//@{
void hash_map::create(std::size_t sz) {
  BOOST_ASSERT( !m_array );

  // round up
  std::size_t const logsz = 1 + nbits(sz);
  sz  = 1 << logsz;

  m_logsz = logsz;
  m_mask  = sz - 1;

  // use an over-sized array -> sentinel bucket
  m_array.reset( new bucket_t[sz + 1] );

  BOOST_ASSERT( m_array[m_mask+1].empty() );
}
//@}
