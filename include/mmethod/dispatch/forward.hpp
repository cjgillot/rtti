//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_FORWARD_HPP
#define RTTI_MMETHOD_DISPATCH_FORWARD_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"

namespace rtti {
namespace mmethod {

/// main dispatch function
template<typename Policy, typename Tag, typename Ret>
struct dispatch {
  inline dispatch();

  template<typename Tuple>
  inline invoker_t MMETHOD_ATTRIBUTE_PURE fetch(Tuple const& args) const;

  template<typename K, typename F>
  inline void insert(F const& f);

  template<typename K>
  inline void generate() const;

private:
  // must only be called by ctor
  static void initialize();

  // must only be called by generate
  static void seal();
};

} // namespace mmethod
} // namespace rtti

#endif
