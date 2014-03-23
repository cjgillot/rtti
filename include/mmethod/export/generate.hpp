//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_TABLE_GENERATE_HPP
#define RTTI_MMETHOD_TABLE_GENERATE_HPP

#include "mmethod/export/table.hpp"

#include <boost/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace mmethod {
namespace detail {

//! structure used for table generation
struct seal_table_type {
  invoker_table_type& table;
  poles_map_type** poles;
};

void init_table(std::size_t arity, detail::invoker_table_type& tbl);
void inse_table(std::size_t arity, invoker_table_type& table, invoker_t inv, rtti_hierarchy* hiers);
void seal_table(std::size_t arity, invoker_table_type  table, seal_table_type& seal);

}}} // namespace rtti::mmethod::detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
