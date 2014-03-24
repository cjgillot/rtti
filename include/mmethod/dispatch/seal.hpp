//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_SEAL_HPP
#define RTTI_MMETHOD_DISPATCH_SEAL_HPP

#include "mmethod/dispatch/forward.hpp"
#include "mmethod/dispatch/fetch.hpp"

#include "mmethod/detail/access.hpp"

#include "mmethod/export/table.hpp"

#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

namespace {

template<std::size_t Arity, typename Tag, std::size_t BTS>
struct seal_poles {
  template<std::size_t J>
  static void eval(poles_map_type** p) {
    enum { ok = BTS & 1 };
    if( ok ) {
      *p = &get_poles_map<ok>::template get<Tag, J>();
      ++p;
    }

    seal_poles<Arity, Tag, (BTS>>1)>::template eval<J+1>(p);
  }
};
template<std::size_t Arity, typename Tag>
struct seal_poles<Arity, Tag, 0> {
  template<std::size_t J>
  static void eval(poles_map_type**) {}
};

} // namespace <>

template<typename Tag, typename Ret>
void dispatch<Tag,Ret>::seal() {
  enum {
    arity = access::traits<Tag>::vsize,
    btset = access::traits<Tag>::type_bitset
  };

  poles_map_type* poles [ arity ];
  seal_table_type seal_table = { get_register<Tag>::invoker_table, poles };

  seal_poles<arity, Tag, btset>::template eval<0>( poles );

  detail::seal_table(arity, get_register<Tag>::invoker_table, seal_table);
}

}}} // namespace rtti::mmethod::detail

#endif
