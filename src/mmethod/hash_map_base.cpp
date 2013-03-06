#include "rtti/hash/hash_map.hpp"

#include "util/assert.hpp"

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::hash::detail::bucket_t;
using rtti::hash::detail::hash_map_base;

#ifndef __EXCEPTIONS__
#define try if(true)
#define catch(...) else if(false)
#endif

#define BADBUCKET (&(m_array[m_mask + 1]))

/// bucket_t implementation
//@{
// inline void bucket_t::reset() { value = 1; }
inline void bucket_t::set(rtti_type k, std::uintptr_t v) {
  value = v;
  key = k;
  ASSERT( !empty() );
}
//@}

/// hash_map_base implementation
//@{
hash_map_base::~hash_map_base() {}

void hash_map_base::move(hash_map_base& o) noexcept {
  ASSERT( this != &o );

  m_mask = o.m_mask;
  m_logsz= o.m_logsz;

  m_array= std::move(o.m_array);
}

void hash_map_base::import(hash_map_base const& o) noexcept {
  ASSERT( this != &o );

  m_array.reset();

  create(1 + o.m_logsz);

  std::size_t sz = 1 << o.m_logsz;
  for(std::size_t i = 0; i < sz; ++i)
    if( !o.m_array[i].empty() )
      insert(key_type( o.m_array[i].key ), o.m_array[i].value);
}
//@}

namespace {
// helper function for [do_find] and [insert]
template<typename Pred>
inline bucket_t* ATTRIBUTE_PURE
probe_table(bucket_t* const m_array, std::size_t index, Pred pred) noexcept {
  // use linear probing
  bucket_t* ptr = &m_array[index];

  // empty bucket sentinel is last of m_array -> forces stop since empty
  while(
      (! ptr->empty())
    & (! pred(ptr))
  ) {
    ++ptr;
  }

  return ptr;
}
} // namespace <anonymous>

/// hash_map_base find()
//@{
hash_map_base::iterator
hash_map_base::do_find(rtti_type key) const noexcept {
  return probe_table(
    m_array.get(), hash(key),
    [&key](bucket_t const* b) { return b->key == key; }
  );
}
//@}

/// hash_map_base insert()
//@{
// [it] has been returned by [find(key, value)]
void hash_map_base::insert_at(iterator it, rtti_type key, std::size_t value) {
  if( !it->empty() )
    return insert(key, value);

  if(LIKELY( it != BADBUCKET ))
    return it->set(key, value);

  // [badbucket] is returned by find
  // only if probing fails -> no need to retry
  insert_need_resize(key, value);
}

void hash_map_base::insert(rtti_type key, std::uintptr_t value) {
  index_type index = hash(key);

  bucket_t* bucket = &m_array[index];
  if(LIKELY( bucket->empty() )) {
    bucket->set(key, value);
    return;
  }

  bucket = probe_table(
    m_array.get(), index,
    [&key](bucket_t const* b) { return b->key == key; }
  );
  if(bucket == BADBUCKET)
    return insert_need_resize(key, value);

  bucket->set(key, value);
}
//@}

/// hash_map_base resize()
//@{
void hash_map_base::insert_need_resize(key_type key, std::uintptr_t value) {
  std::size_t old_size = 1 << m_logsz;

  hash_map_base repl;
  repl.import(*this);
  repl.insert(key, value);
  move(repl);
}
//@}

/// hash_map_base create()
//@{
void hash_map_base::create(std::size_t logsz) {
  ASSERT( !m_array );

  std::size_t sz = 1 << logsz;
  m_logsz = logsz;
  m_mask = sz - 1;

  // use an over-sized array -> sentinel bucket
  m_array.reset( new bucket_t[sz + 1] );
}
//@}
