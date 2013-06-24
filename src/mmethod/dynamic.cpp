#include "rtti/dynamic/poles.hpp"
#include "rtti/dynamic/dynamic.hpp"

#include "util/stw_lock.hpp"
#include "util/attribute.hpp"

#include <type_traits>
#include <boost/functional/hash/hash.hpp>

using namespace rtti;
using namespace rtti::dmethod;
using namespace rtti::dmethod::detail;

using rtti::dmethod::detail::functor_t;
using rtti::dmethod::detail::invoker_entry;
using rtti::dmethod::detail::poles_map_type;
using rtti::dmethod::detail::invoker_table_type;

// pole stuff
//@{
void
rtti::dmethod::detail::init_pole(poles_map_type& map) {
  map.mem.create<1>();
  map.mem.insert( 0, 0 );

  map.poles.create<1>();
  map.poles.insert( 0, 0 );

  ASSERT( map.smallint == 0 );
}

void
rtti::dmethod::detail::init_pole_unary(poles_map_type& map) {
  map.mem.create<1>();
  map.mem.insert( 0, (std::uintptr_t)nullptr );

  map.poles.create<1>();
  map.poles.insert( 0, (std::uintptr_t)nullptr );

  ASSERT( map.smallint == 0 );
}

std::uintptr_t
rtti::dmethod::detail::insert_pole(
  poles_map_type& map
, rtti_node const* hier
) {
#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { map.lock };
#endif

  auto it = map.poles.find( hier->id );

  if( !it->empty() )
    // existing pole, nothing to do
    return it->value;

  std::uintptr_t ret = ++map.smallint;
  map.poles.insert_at( it, hier->id, ret );
  map.mem.flush( map.poles );
  return ret;
}

void
rtti::dmethod::detail::insert_pole_unary(
  poles_map_type& map
, rtti_node const* hier
, functor_t inv
) {
#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { map.lock };
#endif

  auto it = map.poles.find( hier->id );

  if( it->value == (std::uintptr_t)inv )
    // existing pole, nothing to do
    return;

  map.poles.insert_at( it, hier->id, (std::uintptr_t)inv );
  map.mem.flush( map.poles );
}

std::uintptr_t
rtti::dmethod::detail::retract_pole(
  poles_map_type& map
, rtti_node const* hier
) {
#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { map.lock };
#endif

  auto it = map.poles.find( hier->id );

  if( it->empty() )
    // non-existing pole, nothing to do
    return 0;

  std::uintptr_t ret = it->value;
  map.poles.erase( it );
  map.mem.flush( map.poles );
  return ret;
}

functor_t
rtti::dmethod::detail::retract_pole_unary(
  poles_map_type& map
, rtti_node const* hier
) {
  return (functor_t)retract_pole(map, hier);
}
//@}

// entry stuff
//@{
struct rtti::dmethod::detail::invoker_entry {
  invoker_entry* next;
  functor_t ptr;
  rtti_type types[1];
};
static invoker_entry* make_entry(std::size_t arity) {
  invoker_entry* p = (invoker_entry*)std::malloc(sizeof(invoker_entry) + (arity-1) * sizeof(rtti_type));
  return p;
}
static void free_entry(invoker_entry* p) {
  std::free(p);
}
//@}

// dispatch resolution
//@{
constexpr std::size_t max_distance = std::numeric_limits<std::size_t>::max();

static std::size_t ATTRIBUTE_PURE hdistance(rtti_type type, rtti_node const* hier) {
  std::size_t ret = 0;

  while(hier->id != type) {
    ++ret;
    hier = hier->base;
  }

  return hier->id == type
  ? ret
  : max_distance;
}

static bool ATTRIBUTE_PURE better_match_entry(
  std::size_t arity
, invoker_entry* e
, std::size_t* &d1, std::size_t* &d2
, rtti_node const** hiers
) noexcept {
  for(std::size_t i = 0; i < arity; ++i) {
    d2[i] = hdistance(e->types[i], hiers[i]);

    if(d2[i] > d1[i])
      goto fail;
  }

success:
  std::swap(d1, d2);
  return true;

fail:
  return false;
}

static functor_t ATTRIBUTE_PURE
lookup_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_node const** hiers
) {
  invoker_entry* e = table.entries;
  std::vector<invoker_entry*> candidates;

  for(; e; e = e->next) {
    std::unique_ptr<std::size_t[]> d1 { new std::size_t [ arity ] }; // current best
    std::unique_ptr<std::size_t[]> d2 { new std::size_t [ arity ] }; // buffer
    std::fill(&d1[0], &d1[arity], max_distance);

    std::size_t *pd1 = d1.get(), *pd2 = d2.get();

    if( better_match_entry(arity, e, pd1, pd2, hiers) )
      candidates.push_back(e);
  }

  if( candidates.size() == 1 )
    return candidates.front()->ptr;

  return nullptr;
}
//@}

typedef invoker_table_type::key_type key_type;

// hash / equal
//@{
std::size_t ATTRIBUTE_PURE
invoker_table_type::hasher::operator()(key_type const& a) const {
  return boost::hash_range(a.get(), a.get() + *arity);
}
bool ATTRIBUTE_PURE
invoker_table_type::equal::operator()(key_type const& a, key_type const& b) const {
  for(std::size_t i = 0, art = *arity; i < art; ++i)
    if(a.get()[i] != b.get()[i])
      return false;
  return true;
}
//@}

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

static key_type copy_key(std::size_t arity, std::uintptr_t* k) {
  key_type p { new std::uintptr_t[ arity ] };
  std::copy( &p[0], &p[arity], k );
  return std::move(p);
}

functor_t
rtti::dmethod::detail::lookup(
  std::size_t arity
, invoker_table_type& table
, std::uintptr_t* spec
, rtti_node const** hiers
) noexcept {
#if MMETHOD_USE_THREAD
  util::stw_lock::fetch_guard lock { table->lock };
#endif

  shadow_unique_ptr<std::uintptr_t[]> p { spec };
  auto it = table.mem.find( p );

  if(it != table.mem.end())
    return it->second;

#if MMETHOD_USE_THREAD
  util::stw_lock::convert_guard lock2 { table->lock };
#endif

  functor_t ret = lookup_nocache(arity, table, hiers);

  table.mem.emplace( copy_key(arity, spec), ret );

  return ret;
}

void
rtti::dmethod::detail::init_table(
  std::size_t arity
, invoker_table_type& table
) {
  table.entries = make_entry(arity);
  table.entries->next = nullptr;
  table.entries->ptr = nullptr;
  for(std::size_t i = 0; i < arity; ++i)
    table.entries->types[i] = 0;

  table.arity = arity;
}

void
rtti::dmethod::detail::insert(
  std::size_t arity
, invoker_table_type& table
, functor_t inv
, std::uintptr_t* spec
, rtti_node const** hiers
) {

#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { table->lock };
#endif

  table.poles.emplace( copy_key(arity, spec), inv );

  table.mem.clear();
  for(auto& p : table.poles)
    table.mem.emplace( copy_key(arity, p.first.get()), p.second );
}

functor_t
rtti::dmethod::detail::retract(
  std::size_t arity
, invoker_table_type& table
, std::uintptr_t* spec
, rtti_node const** hiers
) {

#if MMETHOD_USE_THREAD
  util::stw_lock::lock_guard lock { table->lock };
#endif

  shadow_unique_ptr<std::uintptr_t[]> p { spec };
  auto it = table.poles.find( p );

  if(it == table.poles.end())
    return nullptr;

  functor_t ret = it->second;
  table.poles.erase(it);
  table.mem.clear();
  for(auto& p : table.poles)
    table.mem.emplace( copy_key(arity, p.first.get()), p.second );

  return ret;
}
