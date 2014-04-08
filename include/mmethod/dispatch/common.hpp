//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_COMMON_HPP
#define RTTI_MMETHOD_DISPATCH_COMMON_HPP

#include "mmethod/config.hpp"

#include "mmethod/dispatch/forward.hpp"
#include "mmethod/dispatch/arity_loop.hpp"

#include "mmethod/detail/access.hpp"

#include "mmethod/export/table.hpp"

#include "mmethod/rtti/getter.hpp"

namespace rtti {
namespace mmethod {
namespace dispatch_detail {

using detail::poles_map_type;
using detail::get_register;

struct get_poles_map {
  template<typename Tag, std::size_t J>
  static poles_map_type* get()
  { return &get_register<Tag>::template poles<J>::array; }
};

}}} // rtti::mmethod::dispatch_detail

#endif
