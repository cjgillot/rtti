//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "wanderer.hpp"

#include "foreach.hpp"

using namespace rtti_dispatch;

wanderer_t::wanderer_t() {
#ifndef NDEBUG
  processing = false;
#endif
}

void
wanderer_t::push_back(rtti_hierarchy k) {
#ifndef NDEBUG
  BOOST_ASSERT( !processing );
#endif

  stack.push_back(k);
}

rtti_hierarchy
wanderer_t::pop() {
#ifndef NDEBUG
  processing = true;
#endif

  for(;;) {
    // exit condition
    if(stack.empty()) {
      return NULL;
    }

    // get next element
    rtti_hierarchy top = stack.back();
    stack.pop_back();

    // already traversed ?
    if(visited.count(top)) {
      continue;
    }

    // inject base classes
    bool const need_upcast = reinject_bases(top);

    // retry if a base has been injected
    if(need_upcast) {
      stack.push_front(top);
      continue;
    }

    // mark as traversed
    visited.insert(top);
    foreach_base(rtti_hierarchy b, top) {
      BOOST_ASSERT(visited.count(b)); (void)b;
    }

    return top;
  }
}

//!\brief Called when an upwards branch needs to be
//! added to the queue for processing.
//!\return true if anything added
bool
wanderer_t::reinject_bases(rtti_hierarchy top_pole) {
  bool need_upcast = false;

  foreach_base(rtti_hierarchy next, top_pole) {
    // not visited yet
    if(!visited.count(next)) {
      stack.push_back(next);
      need_upcast = true;
    }
  }

  return need_upcast;
}
