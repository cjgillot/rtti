#ifndef RTTI_DMETHOD_COMMON_HPP
#define RTTI_DMETHOD_COMMON_HPP

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
