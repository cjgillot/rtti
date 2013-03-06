#include "rtti_allhash/multimethod/poles.hpp"
#include "rtti_allhash/hash.hpp"

#include "util/assert.hpp"

#include <algorithm>
#include <functional>

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::mmethod::detail::map_type_base;

// statistic tools
#if 0

struct statistic_t {
  std::size_t nb_trivial;
  std::size_t nb_upcast;
  std::size_t nb_search;
  std::size_t nb_loop;
} statistic;

#define TRIVIAL() ++statistic.nb_trivial
#define UPCAST()  ++statistic.nb_upcast
#define SEARCH()  ++statistic.nb_search
#define LOOP()    ++statistic.nb_loop

#else

#define TRIVIAL()
#define UPCAST()
#define SEARCH()
#define LOOP()

#endif

/// map_type implementation
//@{
map_type_base::~map_type_base() { delete[] m_array; }

void map_type_base::create(std::size_t sz) {
  // need a power of 2
  ASSERT( !(sz & (sz-1)) );
  ASSERT( !m_array );

  m_array = new bucket_t [ sz ];
  m_bucket_count = sz;
  mask = sz - 1;
}

void map_type_base::insert(std::size_t key, std::size_t value) {
  if( key < m_smallcount ){
    bucket_t* bucket = m_smallarray + key;
    bucket->empty = false;
    bucket->key = key;
    bucket->value = value;
    return;
  }

  key &= ((std::size_t)1 << bucket_t::key_size)-1;

  bucket_t* bucket = m_array + (key & mask);
  if(LIKELY( bucket->empty )) {
    bucket->empty = false;
    bucket->key = key;
    bucket->value = value;
    return;
  }

  // already exists
  if(bucket->key == key)
    return;

  // use quadratic probing : (j^2 + j) / 2
  std::size_t index = key;
  std::size_t j = 0;
  do {
    ++j;
    if(j >= m_bucket_count / 2)
      insert_need_resize(index, value);

    index += j;
    bucket = m_array + (index & mask);

  }
  while(!(bucket->empty));

  bucket->empty = false;
  bucket->key = key;
  bucket->value = value;
}

void map_type_base::insert_at(iterator it, std::size_t key, std::size_t value) {
  ASSERT(( (key &= ((std::size_t)1 << bucket_t::key_size)-1), it->key == key ));
  ASSERT(it->empty);

  if(LIKELY( it != &badbucket )) {
    it->empty = false;
    it->key = key;
    it->value = value;
    return;
  }

  insert(key, value);
}

map_type_base::iterator map_type_base::find(std::size_t key) const {
  if( key < m_smallcount )
    return m_smallarray + key;

  key &= ((std::size_t)1 << bucket_t::key_size)-1;

  bucket_t* bucket = m_array + (key & mask);
  if(LIKELY( !bucket->empty ) & (bucket->key == key))
    return bucket;

  // use quadratic probing : (j^2 + j) / 2
  std::size_t index = key;
  std::size_t j = 0;
  do {
    ++j;
    if(j >= m_bucket_count / 2)
      return const_cast<iterator>(&badbucket);

    index += j;
    bucket = m_array + (index & mask);

    if(UNLIKELY( bucket->empty ))
      return bucket;
  }
  while(bucket->key != key);

  return bucket;
}

void map_type_base::assign(initializer_list_type const& array) {
  const_cast<map_type_base*>(this)->create(next_2exp( array.size() ) * 2);
  for(bucket_t const& a : array)
    const_cast<map_type_base*>(this)->insert(a.key, a.value);
}

void map_type_base::insert_need_resize(std::size_t index, std::size_t value) {
  std::size_t old_size = m_bucket_count;
  std::unique_ptr<bucket_t[]> old { m_array };
  m_array = nullptr;

  create(m_bucket_count * 2);

  for(std::size_t i = 0; i < old_size; ++i)
    if(!old[i].empty)
      insert(old[i].key, old[i].value);

  insert(index, value);
}
//@}

std::size_t
rtti::mmethod::detail::fetch_pole(
  map_type_base& map
, const rtti_node* rt
) throw() {
  const rtti_type id0 = rt->id;
  const map_type_base::iterator it0 = map.find(id0);

  if(LIKELY( !it0->empty ))
    return it0->value;

  do {
    map_type_base::iterator it = map.find(rt->id);

    if(LIKELY( !it->empty ))
    {
      map.insert_at( it0, id0, it->value );
      return it->value;
    }

    rt = rt->base;
  } while(rt);

  return map.find(0)->value;
}
