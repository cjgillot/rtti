//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_TABLE_TABLE_HPP
#define RTTI_MMETHOD_TABLE_TABLE_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/table/hash_map.hpp"

#include <boost/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace mmethod {
namespace detail {

//! useful typedefs
typedef hash::detail::hash_map poles_map_type;
typedef invoker_t* invoker_table_type;

//! structure holding tables
template<typename Tag>
struct register_base {

  template<std::size_t> struct poles {
    static detail::poles_map_type array;
  };

  static void do_initialize();
  static detail::invoker_table_type invoker_table;

};

}}} // namespace rtti::mmethod::detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
