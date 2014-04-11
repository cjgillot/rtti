//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_DISPATCH_FETCH_HPP
#define BOOST_MMETHOD_DISPATCH_FETCH_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/dispatch/common.hpp"

#include <functional>
#include <boost/fusion/include/at_c.hpp>

namespace boost {
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

    rtti_hierarchy const h  = boost::mmethod::get_node(
      boost::fusion::at_c<J>(tuple)
    );

    m += hash_detail::fetch_pole(map, h);
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
    return get_register<Tag>::table()[spec / 2];
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
template<typename Tag, typename Ret>
template<typename Tuple>
invoker_t dispatch<Tag, Ret>::fetch(Tuple const& args) const {
  this->generate();

  enum {
    arity = detail::access::traits<Tag>::vsize,
    btset = detail::access::traits<Tag>::type_bitset
  };

  uintptr_t spec = dispatch_detail::fetch_poles  <arity, Tag, btset>::eval( args );
  invoker_t ret  = dispatch_detail::fetch_invoker<arity, Tag, btset>::eval( spec );
  BOOST_ASSERT(ret);

  return ret;
}

}} // boost::mmethod

#endif
