//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_GENERATE_HPP
#define RTTI_MMETHOD_DISPATCH_GENERATE_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"
#include "mmethod/dispatch/forward.hpp"

namespace rtti {
namespace mmethod {

/// main dispatch function
template<typename Tag, typename Ret>
dispatch<Tag, Ret>::dispatch() {
  struct initializer_t {
    initializer_t() { dispatch::initialize(); }
    void touch() {}
  }
  static initializer;
  initializer.touch();
}

template<typename Tag, typename Ret>
void dispatch<Tag, Ret>::generate() const {
  struct sealer_t {
    sealer_t() { dispatch::seal(); }
    void touch() {}
  }
  static sealer;
  sealer.touch();
}

}} // rtti::mmethod

#endif
