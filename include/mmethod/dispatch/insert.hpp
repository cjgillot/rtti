//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_INSERT_HPP
#define RTTI_MMETHOD_DISPATCH_INSERT_HPP

#include "mmethod/config.hpp"
#include "mmethod/dispatch/common.hpp"

#include "mmethod/detail/mpl.hpp"

#include <boost/move/unique_ptr.hpp>

namespace rtti {
namespace mmethod {

namespace dispatch_detail {

template<typename Tuple>
struct save_poles_once {
  rtti_hierarchy* h;

  template<std::size_t J>
  void apply() {
    typedef typename boost::mpl::at_c<Tuple, J>::type arg_type;

    *h = ::rtti::static_node< arg_type >();
    ++h;
  }
};
template<std::size_t BTS>
struct save_poles {
  template<typename Tuple>
  static void eval(rtti_hierarchy* h, Tuple* /*unused*/) {
    save_poles_once<Tuple> fetcher = { h };

    arity_loop<BTS>::apply(fetcher);
  }
};

} // namespace dispatch_detail

template<typename Tag>
template<typename K, typename F>
void dispatch<Tag>::insert(F const& f) {
  typedef detail::access::traits<Tag> traits_type;

  enum {
    arity = traits_type::virtual_size
  , btset = traits_type::tags_bitset
  };
  rtti_hierarchy hiers [ arity ];

  dispatch_detail::save_poles<btset>::eval( hiers, static_cast<K*>(NULL) );

  invoker_t inv = reinterpret_cast<invoker_t>(f);

  detail::inse_table(
    arity
  , dispatch_detail::get_register<Tag>::early()
  , inv
  , hiers
  );
}

} // namespace mmethod
} // namespace rtti

#endif
