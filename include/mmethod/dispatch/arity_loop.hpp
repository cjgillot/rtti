//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_ARITY_LOOP_HPP
#define RTTI_MMETHOD_DISPATCH_ARITY_LOOP_HPP

#include "mmethod/config.hpp"

namespace rtti {
namespace mmethod {
namespace dispatch_detail {

template<std::size_t J, std::size_t BTS, bool Virt = BTS & 1>
struct arity_loop_helper {
  template<typename Func>
  BOOST_FORCEINLINE static void apply(Func& f) {
    f.template apply<J>();
    arity_loop_helper<J+1, (BTS>>1)>::apply(f);
  }
};

template<std::size_t J, std::size_t BTS>
struct arity_loop_helper<J, BTS, false> {
  template<typename Func>
  BOOST_FORCEINLINE static void apply(Func& f) {
    arity_loop_helper<J+1, (BTS>>1)>::apply(f);
  }
};

template<std::size_t J>
struct arity_loop_helper<J, 0, false> {
  template<typename Func>
  BOOST_FORCEINLINE static void apply(Func& /*unused*/) {}
};

template<std::size_t BTS>
struct arity_loop {
  template<typename Func>
  BOOST_FORCEINLINE static void apply(Func& f) {
    arity_loop_helper<0, BTS>::apply(f);
  }
};

} // namespace dispatch_detail
} // namespace mmethod
} // namespace rtti

#endif
