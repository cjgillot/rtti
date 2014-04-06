//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_COMMON_HPP
#define RTTI_MMETHOD_DISPATCH_COMMON_HPP

#include "boost/mmethod/config.hpp"

#include "boost/mmethod/dispatch/forward.hpp"
#include "boost/mmethod/dispatch/arity_loop.hpp"

#include "boost/mmethod/detail/access.hpp"

#include "boost/mmethod/export/table.hpp"

#include "boost/mmethod/rtti/getter.hpp"

namespace boost {
namespace mmethod {
namespace detail {

struct get_poles_map {
  template<typename Tag, std::size_t J>
  static poles_map_type* get()
  { return &get_register<Tag>::template poles<J>::array; }
};

}}} // boost::mmethod::detail

#endif
