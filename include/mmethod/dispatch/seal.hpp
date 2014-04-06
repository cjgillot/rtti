//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_DISPATCH_SEAL_HPP
#define BOOST_MMETHOD_DISPATCH_SEAL_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/dispatch/common.hpp"

namespace boost {
namespace mmethod {
namespace detail {

namespace {

template<typename Tag>
struct seal_poles_once {
  poles_map_type** p;

  template<std::size_t J>
  void apply() {
    *p = get_poles_map::get<Tag, J>();
    ++p;
  }
};
template<typename Tag, std::size_t BTS>
struct seal_poles {
  static void eval(poles_map_type** h) {
    seal_poles_once<Tag> fetcher = { h };

    arity_loop<BTS>::apply(fetcher);
  }
};

} // namespace <>

template<typename Tag, typename Ret>
void dispatch<Tag,Ret>::seal() {
  enum {
    arity = access::traits<Tag>::vsize
  , btset = access::traits<Tag>::type_bitset
  };

  poles_map_type* poles [ arity ];
  seal_table_type seal_table = { get_register<Tag>::invoker_table, poles };

  seal_poles<Tag, btset>::eval( poles );

  detail::seal_table(arity, get_register<Tag>::invoker_table, seal_table);
}

}}} // namespace boost::mmethod::detail

#endif
