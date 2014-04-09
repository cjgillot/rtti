//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_TABLE_GENERATE_HPP
#define RTTI_MMETHOD_TABLE_GENERATE_HPP

#include "mmethod/config.hpp"
#include "mmethod/export/table/table.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace mmethod {
namespace detail {

typedef void (*ambiguity_handler_t)(size_t n, rtti_type const[]);

struct ambiguity_policy_t {
  ambiguity_handler_t   ahndl;
  invoker_t             bad_dispatch;
};

//! structure used for table generation
struct seal_table_type {
  invoker_table_type& table;
  poles_map_type** poles;
  ambiguity_policy_t& ambiguity_policy;
};

void init_table(std::size_t arity, invoker_table_type& tbl);
void inse_table(std::size_t arity, invoker_table_type& table, invoker_t inv, rtti_hierarchy* hiers);
void seal_table(std::size_t arity, invoker_table_type  table, seal_table_type& seal);

}}} // namespace rtti::mmethod::detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
