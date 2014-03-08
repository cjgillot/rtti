#ifndef RTTI_DMETHOD_POLES_HPP
#define RTTI_DMETHOD_POLES_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/hash/hash_map/hash_map.hpp"

#include <unordered_map>

namespace rtti {
namespace dmethod {
namespace detail {

using hash::detail::poles_map_type;

typedef invoker_t* invoker_table_type;

struct seal_table_type {
  invoker_table_type& table;
  poles_map_type** poles;
};

void init_table(std::size_t arity, detail::invoker_table_type& tbl);
void inse_table(std::size_t arity, invoker_table_type& table, invoker_t inv, rtti_hierarchy* hiers);
void seal_table(std::size_t arity, invoker_table_type  table, seal_table_type& seal);

}}} // namespace rtti::mmethod::detail

#endif
