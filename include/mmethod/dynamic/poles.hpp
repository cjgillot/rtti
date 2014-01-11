#ifndef RTTI_DMETHOD_POLES_HPP
#define RTTI_DMETHOD_POLES_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/hash/hash_map/hash_map.hpp"

#if MMETHOD_USE_THREAD
#include "util/stw_lock.hpp"
#endif

#include <unordered_map>

namespace rtti {
namespace dmethod {
namespace detail {

typedef void* functor_t;

struct poles_map_type {
  hash::detail::hash_map mem;
  hash::detail::hash_map poles;

  std::size_t smallint = 0;

#if MMETHOD_USE_THREAD
  util::stw_lock lock;
#endif
};

struct invoker_entry;
struct invoker_table_type {
  struct mem_table;
  struct poles_table;

  mem_table*   cache;
  poles_table* root;

#if MMETHOD_USE_THREAD
  util::stw_lock lock;
#endif
};


functor_t ATTRIBUTE_PURE lookup(
  std::size_t arity
, invoker_table_type& table
, std::uintptr_t* spec
, rtti_hierarchy* hiers
) noexcept;

void init_pole      (detail::poles_map_type& map);
void init_pole_unary(detail::poles_map_type& map);
void init_table(std::size_t arity, detail::invoker_table_type& tbl);

std::uintptr_t insert_pole       (poles_map_type& map, rtti_hierarchy hier);
void           insert_pole_unary (poles_map_type& map, rtti_hierarchy hier, functor_t inv);
void           insert (std::size_t arity, invoker_table_type& table, functor_t inv, std::uintptr_t* spec, rtti_hierarchy* hiers);

std::uintptr_t retract_pole      (poles_map_type& map, rtti_hierarchy hier);
functor_t      retract_pole_unary(poles_map_type& map, rtti_hierarchy hier);
functor_t      retract(std::size_t arity, invoker_table_type& table, std::uintptr_t* spec, rtti_hierarchy* hiers);

}}} // namespace rtti::mmethod::detail

#endif
