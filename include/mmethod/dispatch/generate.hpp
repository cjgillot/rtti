//          Copyright Camille Gillot 2012 - 2015.
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
template<typename RootArgs>
void dispatch<Tag, Ret>::generate() const {
  struct sealer_t {
    explicit sealer_t(dispatch const* self) {
      // insert defining signature
      // to avoid incomplete generation
      // (weird stuff can happen if it is not implemented)
      const_cast<dispatch*>(self)->template insert<RootArgs, invoker_t>(NULL);

      dispatch::seal();
    }
    void touch() {}
  }
  static sealer ( this );
  sealer.touch();
}

}} // rtti::mmethod

#endif
