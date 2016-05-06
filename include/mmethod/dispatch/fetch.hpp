//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_FETCH_HPP
#define RTTI_MMETHOD_DISPATCH_FETCH_HPP

#include "mmethod/config.hpp"
#include "mmethod/dispatch/common.hpp"

#include <boost/tuple/tuple.hpp>

namespace rtti {
namespace mmethod {

namespace dispatch_detail {

/// fetch_poles<>::eval(spec,_,args) loops over args and returns the sum of pole-data
/// equivalent pseudo-code :
/// \code
/// i = 0
/// for each arg,
///   if arg is virtual
///     i += arg->pole->data
/// \endcode
template<typename Tag, typename Tuple>
struct fetch_poles_once {
  uintptr_t& m;
  Tuple const& tuple;

  template<std::size_t J>
  void apply() const {
    poles_map_type& map = get_register<Tag>::template poles<J>();

    rtti_hierarchy const h  = ::rtti::get_node(
      tuple.template get<J>()
    );

    m += hash::detail::fetch_pole(map, h);
  }
};
template<std::size_t Arity, typename Tag, std::size_t BTS>
struct fetch_poles {
  template<typename Tuple>
  static uintptr_t eval(Tuple const& args) {
    uintptr_t result = 0;

    fetch_poles_once<Tag, Tuple> fetcher = { result, args };
    arity_loop<BTS>::apply(fetcher);

    return result;
  }
};

template<std::size_t Arity, typename Tag, std::size_t BTS>
struct fetch_invoker {
  static invoker_t eval(uintptr_t spec) {
    return get_register<Tag>::table()[spec];
  }
};
template<typename Tag, std::size_t BTS>
struct fetch_invoker<1, Tag, BTS> {
  static invoker_t eval(uintptr_t spec) {
    return (invoker_t)spec;
  }
};

} // namespace dispatch_detail

/// main dispatch function
template<typename Tag>
template<typename Tuple>
invoker_t dispatch<Tag>::fetch(Tuple const& args) const {
  // verify we have sealed the table
  BOOST_ASSERT(detail::get_register<Tag>::early() == NULL);
  typedef detail::access::traits<Tag> traits_type;

  enum {
    arity = traits_type::virtual_size,
    btset = traits_type::tags_bitset
  };

  uintptr_t spec = dispatch_detail::fetch_poles  <arity, Tag, btset>::eval( args );
  invoker_t ret  = dispatch_detail::fetch_invoker<arity, Tag, btset>::eval( spec );
  BOOST_ASSERT(ret);

  return ret;
}

} // namespace mmethod
} // namespace rtti

#endif
