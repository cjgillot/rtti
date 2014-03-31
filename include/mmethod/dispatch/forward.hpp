//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_FORWARD_HPP
#define RTTI_MMETHOD_DISPATCH_FORWARD_HPP

#include "mmethod/rttifwd.hpp"
#include "mmethod/detail/attribute.hpp"

namespace rtti {
namespace mmethod {
namespace detail {

/// main dispatch function
template<typename Tag, typename Ret>
struct dispatch {
  dispatch() {
    struct initializer_t {
      initializer_t() { dispatch::initialize(); };
      void touch() {}
    }
    static initializer;
    initializer.touch();
  }

  template<typename Tuple>
  invoker_t MMETHOD_ATTRIBUTE_PURE fetch(Tuple const& args) const;

  template<typename K, typename F>
  void insert(F const& f);

  void generate() const {
    struct sealer_t {
      sealer_t() { dispatch::seal(); };
      void touch() {}
    }
    static sealer;
    sealer.touch();
  }

private:
  // must only be called by ctor
  static void initialize();

  // must only be called by generate
  static void seal();
};

}}} // rtti::mmethod::detail

#endif
