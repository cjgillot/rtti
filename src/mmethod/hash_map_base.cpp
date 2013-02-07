#include "rtti/mmethod/hash_map.hpp"
#include "rtti/hash.hpp"

#include "util/assert.hpp"

#include <algorithm>
#include <functional>

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::mmethod::hash_map_detail::bucket_t;
using rtti::mmethod::hash_map_detail::hash_map_base;

#define USE_LINPROB

#ifndef __EXCEPTIONS__
#define try if(true)
#define catch(...) else if(false)
#endif

#define BADBUCKET (&(m_array[m_mask + 1]))

/// bucket_t implementation
//@{
inline void bucket_t::reset() { value = 0; }
inline void bucket_t::set(rtti_type k, std::uintptr_t v) {
  key = k;
  value = v;
  ASSERT( !empty() );
}
//@}

/// hash_map_base implementation
//@{
hash_map_base::~hash_map_base() {}

/// TODO : thread-safe operator=
hash_map_base& hash_map_base::operator=(hash_map_base&& o) noexcept {
  ASSERT( this != &o );

  /// This function specificity :
  /// It is meant to be called while find() operations are being done.
  /// Beware then of the \c m_mask, \c m_logsz and \c m_array updates.

  m_mask = o.m_mask;
  m_logsz= o.m_logsz;
  m_array= std::move(o.m_array);

  return *this;
}
//@}

// helper function for [do_find] and [insert]
template<typename Pred>
inline bucket_t* ATTRIBUTE_PURE
probe_table(bucket_t* const m_array, std::size_t index, std::size_t key, std::size_t m_logsz, std::size_t m_mask, Pred pred) {
  // use linear probing
  bucket_t* ptr = &m_array[index];
  std::size_t n = 0;

  // empty bucket sentinel is last of m_array -> forces stop since empty
  while(
      (! ptr->empty())
    & (! pred(ptr))
  ) {
#ifdef USE_LINPROB
    ++ptr;
    (void)n; (void)key; (void)m_logsz; (void)m_mask;
#else
    if(n > m_mask/2) return BADBUCKET;

    n += 2;
    key >>= m_logsz; key ^= 14923729446516375050ULL;
    index += key; index &= m_mask;
    ptr = &m_array[index];
#endif
  }

  return ptr;
}

/// hash_map_base find()
//@{
hash_map_base::iterator
hash_map_base::do_find(index_type index, rtti_type key) const {
  return probe_table(
    m_array.get(), index, key, m_logsz, m_mask,
    [key](bucket_t const* b) { return b->key == key; }
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

  ASSERT(bucket->key != key);

  bucket = probe_table(
    m_array.get(), index, key, m_logsz, m_mask,
    [](bucket_t const*) { return false; }
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
  repl.do_create(m_logsz + 1);

  for(std::size_t i = 0; i < old_size; ++i)
    if( !m_array[i].empty() )
      repl.insert(key_type( m_array[i].key ), m_array[i].value);

  repl.insert(key, value);

  operator=( std::move(repl) );
}

void hash_map_base::do_create(std::size_t logsz) {
  // need a power of 2
  ASSERT( !m_array );

  std::size_t sz = 1 << logsz;
  m_logsz = logsz;
  m_mask = sz - 1;

  m_array.reset( new bucket_t[sz + 1] );
}
//@}
