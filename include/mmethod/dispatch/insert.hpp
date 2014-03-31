//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_INSERT_HPP
#define RTTI_MMETHOD_DISPATCH_INSERT_HPP

#include "mmethod/dispatch/common.hpp"

#include <boost/mpl/at.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

namespace {

template<typename Tag, typename Tuple>
struct save_poles_once {
  rtti_hierarchy* h;

  template<std::size_t J>
  void apply() {
    typedef typename boost::mpl::at_c<Tuple, J>::type arg_type;

    *h = ::rtti::static_node< arg_type >();
    ++h;
  }
};
template<typename Tag, std::size_t BTS>
struct save_poles {
  template<typename Tuple>
  static void eval(rtti_hierarchy* h, Tuple*) {
    save_poles_once<Tag, Tuple> fetcher = { h };

    arity_loop<BTS>::apply(fetcher);
  }
};

} // namespace <>

template<typename Tag, typename Ret>
template<typename K, typename F>
void dispatch<Tag,Ret>::insert(F const& f) {
  enum {
    arity = access::traits<Tag>::vsize
  , btset = access::traits<Tag>::type_bitset
  };
  rtti_hierarchy hiers [ arity ];

  save_poles<Tag, btset>::eval( hiers, static_cast<K*>(NULL) );

  invoker_t inv = reinterpret_cast<invoker_t>(f);

  detail::inse_table(arity, get_register<Tag>::invoker_table, inv, hiers);
}

}}} // namespace rtti::mmethod::detail

#endif
