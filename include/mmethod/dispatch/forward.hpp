//          Copyright Camille Gillot 2012 - 2014.
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
template<typename Tag, typename Ret>
struct dispatch {
  inline dispatch();

  template<typename Tuple>
  inline invoker_t MMETHOD_ATTRIBUTE_PURE fetch(Tuple const& args) const;

  template<typename K, typename F>
  inline void insert(F const& f);

  inline void generate() const;

private:
  // must only be called by ctor
  inline static void initialize();

  // must only be called by generate
  inline static void seal();
};

}} // rtti::mmethod

#endif
