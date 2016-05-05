//          Copyright Camille Gillot 2012 - 2016.
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

template<typename Tag>
BOOST_NOINLINE
void dispatch<Tag>::initialize() {
  typedef detail::access::traits<Tag> traits_type;

  enum { arity = traits_type::vsize };

  detail::init_table(arity, detail::get_register<Tag>::early());
}

namespace dispatch_detail {

template<typename Register>
struct seal_poles_once {
  poles_map_type** p;

  template<std::size_t J>
  void apply() {
    *p = &Register::template poles<J>();
    ++p;
  }
};
template<typename Register, std::size_t BTS>
struct seal_poles {
  static void eval(poles_map_type** h) {
    seal_poles_once<Register> fetcher = { h };

    arity_loop<BTS>::apply(fetcher);
  }
};

} // namespace dispatch_detail

template<typename Tag>
BOOST_NOINLINE
void dispatch<Tag>::seal() {
  typedef detail::access::traits<Tag> traits_type;

  enum {
    arity = traits_type::vsize
  , btset = traits_type::type_bitset
  };
  typedef detail::get_register<Tag> register_type;

  detail::poles_map_type* poles [ arity ];
  detail::invoker_table_type&  table = register_type::table();
  detail::early_bindings_type& early = register_type::early();

  typedef typename traits_type::policy policy_type;
  typedef ambiguity::policy_traits<policy_type> policy_traits_type;

  typedef typename detail::access::trampoline<Tag>::sig_t fp_t;
  detail::ambiguity_policy_t policy = {
    policy_traits_type::get_ambiguity_handler()
  , policy_traits_type::template get_bad_dispatch<fp_t>()
  };

  detail::seal_table_type seal_table = {
    table
  , poles
  , policy
  };

  dispatch_detail::seal_poles<register_type, btset>::eval( poles );

  detail::seal_table(arity, early, seal_table);
}

} // namespace mmethod
} // namespace rtti

#endif
