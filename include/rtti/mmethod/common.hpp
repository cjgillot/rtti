#ifndef RTTI_MMETHOD_COMMON_HPP
#define RTTI_MMETHOD_COMMON_HPP

#include "rtti/mmethod/poles.hpp"

namespace rtti {
namespace mmethod {

namespace detail { namespace mpl {

template<std::size_t... SEQ> struct mplpack_c;

}} // namespace detail::mpl

void ATTRIBUTE_NORETURN _rtti_bad_dispatch();

template<typename Tag>
struct mmethod_register_base {

  template<std::size_t> struct poles {
    static detail::poles_map_type array;
  };

  static void do_initialize();
  static detail::invoker_t fetch(std::uintptr_t spec);
  static detail::invoker_t const* const invoker_table;

};

}} // namespace rtti::mmethod

#endif
