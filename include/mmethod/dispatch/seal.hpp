//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_SEAL_HPP
#define RTTI_MMETHOD_DISPATCH_SEAL_HPP

#include "mmethod/config.hpp"
#include "mmethod/dispatch/common.hpp"

#include "mmethod/export/table.hpp"

#include "mmethod/policy/utils.hpp"

namespace rtti {
namespace mmethod {

template<typename Tag, typename Ret>
BOOST_NOINLINE
void dispatch<Tag,Ret>::initialize() {
  enum { arity = detail::access::traits<Tag>::vsize };

  detail::init_table(arity, detail::get_register<Tag>::early());
}

namespace dispatch_detail {

template<typename Tag>
struct seal_poles_once {
  poles_map_type** p;

  template<std::size_t J>
  void apply() {
    *p = &get_register<Tag>::template poles<J>();
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

} // namespace dispatch_detail

template<typename Tag, typename Ret>
BOOST_NOINLINE
void dispatch<Tag,Ret>::seal() {
  enum {
    arity = detail::access::traits<Tag>::vsize
  , btset = detail::access::traits<Tag>::type_bitset
  };

  detail::poles_map_type* poles [ arity ];
  detail::invoker_table_type&  table = detail::get_register<Tag>::table();
  detail::early_bindings_type& early = detail::get_register<Tag>::early();

  typedef typename detail::access::traits<Tag>::policy policy_type;
  typedef typename detail::access::trampoline<Tag>::sig_t fp_t;
  detail::ambiguity_policy_t policy = {
    ambiguity::get_fpointers<policy_type>::get_ambiguity_handler()
  , ambiguity::get_fpointers<policy_type>::template get_bad_dispatch<fp_t>()
  };

  detail::seal_table_type seal_table = {
    table
  , poles
  , policy
  };

  dispatch_detail::seal_poles<Tag, btset>::eval( poles );

  detail::seal_table(arity, early, seal_table);
}

}} // namespace rtti::mmethod

#endif
