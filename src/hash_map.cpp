//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/hash_map.hpp"

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::hash::detail::bucket_t;
using rtti::hash::detail::hash_map;
using rtti::hash::detail::key_type;
using rtti::hash::detail::index_type;
using rtti::hash::detail::value_type;

#define BADBUCKET (&(m_array[ m_size ]))

/// hash_map move()
//@{
void hash_map::move(hash_map& o) BOOST_NOEXCEPT_OR_NOTHROW {
  BOOST_ASSERT( this != &o );

  m_mask = o.m_mask;
  m_size = o.m_size ;

  m_array.swap(o.m_array);
}
//@}

/// table probing
//@{
namespace {
// helper function for [do_find] and [insert]
inline bucket_t*
probe_table(bucket_t* const m_array, index_type index, rtti_type key) BOOST_NOEXCEPT_OR_NOTHROW {
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

//@}

/// hash_map find()
//@{
#ifndef MMETHOD_INLINE_DO_FIND
hash_map::iterator
hash_map::do_find(rtti_type key) const BOOST_NOEXCEPT_OR_NOTHROW {
  return probe_table(
    m_array.get(), hash(key), key
  );
}
#endif
//@}

/// hash_map insert()
//@{
void hash_map::insert(key_type key, value_type value) {
  index_type index = hash(key);

  bucket_t* bucket = probe_table(
    m_array.get(), index, key
  );
  if(bucket != BADBUCKET)
    return bucket->set(key, value);

  insert_need_resize(key, value);
}

void hash_map::insert_need_resize(key_type key, value_type value) {
  std::size_t old_sz = m_size;
  std::size_t new_sz = 2 * old_sz;
  hash_map repl;

  repl.create(new_sz);

  for(std::size_t i = 0; i < old_sz; ++i)
    if( !m_array[i].empty() )
      repl.insert(m_array[i].key(), m_array[i].value());
  repl.insert(key, value);

  move(repl);
}
//@}

/// hash_map create()
//@{
inline static std::size_t
nbits(std::size_t n) {
  std::size_t ret = 0;
  for(; n; n>>=1)
    ++ret;
  return ret;
}

void hash_map::create(std::size_t sz) {
  BOOST_ASSERT( !m_array );

  // round up
  std::size_t const logsz = 1 + nbits(sz);
  sz  = 1 << logsz;

  m_size = sz;
  m_mask  = sz - 1;

  // use an over-sized array -> sentinel bucket
  m_array.reset( new bucket_t[sz + 1] );

  BOOST_ASSERT( BADBUCKET->empty() );
}
//@}
