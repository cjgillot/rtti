#ifndef RTTI_DMETHOD_IMPL_HPP
#define RTTI_DMETHOD_IMPL_HPP

#include "rtti/dynamic/poles.hpp"

#if MMETHOD_USE_THREAD
#include "util/stw_lock.hpp"
#endif

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace rtti;
using namespace rtti::dmethod;
using namespace rtti::dmethod::detail;

using rtti::dmethod::detail::functor_t;
using rtti::dmethod::detail::invoker_entry;
using rtti::dmethod::detail::poles_map_type;
using rtti::dmethod::detail::invoker_table_type;

using rtti::detail::rtti_hierarchy;

/// implementation types

typedef functor_t value_type;

void init_nocache(
  std::size_t arity
, invoker_table_type&
);
functor_t lookup_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
);



void init_nocache(
  std::size_t arity
, invoker_table_type& tbl
);
functor_t lookup_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
);
void insert_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
, functor_t f
);
void retract_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
, functor_t f
);

#endif
