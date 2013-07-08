#include "rtti/dynamic/poles.hpp"
#include "rtti/dynamic/dynamic.hpp"

#include "impl.hpp"

#include <boost/functional/hash/hash.hpp>

#include "mm/cache_map.hpp"

typedef std::unique_ptr<std::uintptr_t[]> cache_key_type;
struct cache_hasher {
  std::size_t const arity;
  std::size_t operator()(cache_key_type const& a) const {
    return boost::hash_range(a.get(), a.get() + arity);
  }
};
struct cache_equal {
  std::size_t const arity;
  bool operator()(cache_key_type const& a, cache_key_type const& b) const {
    if(!a.get() || !b.get())
      return false;
    return std::equal(
      a.get(), a.get()+arity
    , b.get()
    );
  }
};

using cache_map = mm::cache_map<cache_key_type, value_type, cache_hasher, cache_equal>;
struct invoker_table_type::mem_table
{
  explicit mem_table(std::size_t arity)
  : map(arity, cache_hasher{arity}, cache_equal{arity}) {}
  
  
  cache_map map;
};

void
rtti::dmethod::detail::init_table(
  std::size_t arity
, invoker_table_type& tbl
) {
  tbl.cache = new invoker_table_type::mem_table(arity);
  init_nocache(arity, tbl);
}

namespace {

template<typename T>
struct shadow_unique_ptr
: std::unique_ptr<T> {
  template<typename... Args>
  shadow_unique_ptr(Args&& ...args)
  : std::unique_ptr<T>( std::forward<Args>(args)... )
  {}

  ~shadow_unique_ptr() { this->release(); }
};

}

static cache_key_type copy_key(std::size_t arity, std::uintptr_t* k) {
  cache_key_type p { new std::uintptr_t[ arity ] };
  std::copy( &p[0], &p[arity], k );
  return p;
}

functor_t
rtti::dmethod::detail::lookup(
  std::size_t arity
, invoker_table_type& table
, std::uintptr_t* spec
, rtti_hierarchy* hiers
) noexcept {
  ASSERT(table.cache);

#if MMETHOD_USE_THREAD
  util::stw_lock::fetch_guard lock { table->lock };
#endif

  shadow_unique_ptr<std::uintptr_t[]> p { spec };
  auto it = table.cache->map.find( p );

  if(it != table.cache->map.end())
    return it->second;

#if MMETHOD_USE_THREAD
  util::stw_lock::convert_guard lock2 { table->lock };
#endif

  functor_t ret = lookup_nocache(arity, table, hiers);

  table.cache->map.emplace( copy_key(arity, spec), ret );

  return ret;
}

void
rtti::dmethod::detail::insert(
  std::size_t arity
, invoker_table_type& table
, functor_t inv
, std::uintptr_t* spec
, rtti_hierarchy* hiers
) {
  ASSERT(table.cache);

#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { table->lock };
#endif

  auto present = table.cache->map.emplace( copy_key(arity, spec), inv );
  if(! present.second)
    // already something
    return;

  insert_nocache(arity, table, hiers, inv);
}

functor_t
rtti::dmethod::detail::retract(
  std::size_t arity
, invoker_table_type& table
, std::uintptr_t* spec
, rtti_hierarchy* hiers
) {
  ASSERT(table.cache);

#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { table->lock };
#endif

  shadow_unique_ptr<std::uintptr_t[]> p { spec };
  auto it = table.cache->map.find( p );

  if(it == table.cache->map.end())
    return nullptr;

  functor_t ret = it->second;
  table.cache->map.clear();

  retract_nocache(arity, table, hiers, ret);
  return ret;
}
