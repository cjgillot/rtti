//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "wanderer.hpp"

#include "foreach.hpp"

using namespace rtti_dispatch;

wanderer_t::wanderer_t() {}

void
wanderer_t::push_back(rtti_hierarchy k) {
  //!Check we have not started iterating.
  BOOST_ASSERT( visited.empty() );

  stack.push_back(k);
}

rtti_hierarchy
wanderer_t::pop() {
  //!Main loop.
  // 'stack' works like a call stack:
  // for each visited class,
  // we check its bases.
  // If a base needs visiting,
  // we push the current class,
  // and iterate among the bases.
  for(;;) {
    // Exit condition
    if(stack.empty()) {
      return NULL;
    }

    // Next element
    rtti_hierarchy top = stack.back();
    stack.pop_back();

    // If we have already traversed it,
    // just ignore it.
    if(visited.count(top)) {
      continue;
    }

    // Check the base classes.
    bool const need_upcast = reinject_bases(top);

    // At least ont base has been added.
    // Push ourself at the bottom of the stack
    // and retry.
    if(need_upcast) {
      stack.push_front(top);
      continue;
    }

    // We have a traversable candidate,
    // mark it as seen and return it.
    visited.insert(top);
#ifndef NDEBUG
    foreach_base(rtti_hierarchy b, top) {
      BOOST_ASSERT(visited.count(b));
    }
#endif

    return top;
  }
}

//!\brief Called when an upwards branch needs to be
//! added to the queue for processing.
//!\return true if anything added
bool
wanderer_t::reinject_bases(rtti_hierarchy top_pole) {
  bool need_upcast = false;

  // Add all unvisited bases to the stack.
  foreach_base(rtti_hierarchy next, top_pole) {
    if(!visited.count(next)) {
      stack.push_back(next);
      need_upcast = true;
    }
  }

  return need_upcast;
}
