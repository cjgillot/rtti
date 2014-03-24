//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_INITIALIZE_HPP
#define RTTI_MMETHOD_DISPATCH_INITIALIZE_HPP

#include "mmethod/dispatch/forward.hpp"

#include "mmethod/detail/access.hpp"

#include "mmethod/export/table.hpp"

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Tag>
invoker_table_type register_base<Tag>::invoker_table;

template<typename Tag>
template<std::size_t J>
poles_map_type register_base<Tag>::poles<J>::array;

template<typename Tag, typename Ret>
void dispatch<Tag,Ret>::initialize() {
  enum { arity = access::traits<Tag>::vsize };

  detail::init_table(arity, get_register<Tag>::invoker_table);
}

}}} // namespace rtti::mmethod::detail

#endif
