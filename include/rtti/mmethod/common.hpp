#ifndef RTTI_MMETHOD_COMMON_HPP
#define RTTI_MMETHOD_COMMON_HPP

#include "rtti/mmethod/poles.hpp"
#include "rtti/shared/basic.hpp"

namespace rtti {

namespace mpl {

template<std::size_t... SEQ> struct mplpack_c;

} // namespace mpl

namespace mmethod {

using rtti::_rtti_bad_dispatch;

template<typename Tag>
struct mmethod_register_base {

  template<std::size_t> struct poles {
    static detail::poles_map_type array;
  };

  static void do_initialize();
  static detail::invoker_table_type invoker_table;

};

}} // namespace rtti::mmethod

#endif
