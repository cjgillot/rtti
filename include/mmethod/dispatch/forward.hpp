//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_FORWARD_HPP
#define RTTI_MMETHOD_DISPATCH_FORWARD_HPP

#include "rtti/rttifwd.hpp"
#include "rtti/detail/attribute.hpp"

namespace rtti {
namespace dmethod {
namespace detail {

/// main dispatch function
template<typename Tag, typename Ret>
struct dispatch {
  dispatch() {
    struct initializer_t {
      initializer_t() { Tag::do_initialize(); };
      void touch() {}
    }
    static initializer;
    initializer.touch();
  }

  template<typename Tuple>
  invoker_t ATTRIBUTE_PURE fetch(Tuple const& args) const;

  template<typename K, typename F>
  void insert(F const& f);

  // calls seal once
  void generate() const {
    struct sealer_t {
      sealer_t(dispatch const* a) { a->seal(); };
      void touch() {}
    }
    static sealer ( this );
    sealer.touch();
  }

  // must only be called by generate
  void seal() const;
};

}}} // rtti::mmethod::detail

#endif
