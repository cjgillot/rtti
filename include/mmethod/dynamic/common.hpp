//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_COMMON_HPP
#define RTTI_MMETHOD_COMMON_HPP

#include "mmethod/dynamic/poles.hpp"
#include "mmethod/shared/basic.hpp"

namespace rtti {
namespace dmethod {

using rtti::_rtti_bad_dispatch;

template<typename Tag>
struct register_base {

  template<std::size_t> struct poles {
    static detail::poles_map_type array;
  };

  static void do_initialize();
  static detail::invoker_table_type invoker_table;

};

}} // namespace rtti::mmethod

#endif
