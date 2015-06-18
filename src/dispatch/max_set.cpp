//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "max_set.hpp"

#include "foreach.hpp"

max_set::max_set() {}

void max_set::insert(const signature_t& sig0, const link_table& links) {
  std::size_t const arity = sig0.array().size();

  for(std::size_t k = 0; k < arity; ++k) {
    klass_t const* kth = sig0.array()[k];

    foreach(klass_t const* base, kth->get_bases()) {
      // copy signature and replace
      signature_t sig = sig0;
      sig.array_ref()[k] = base;

      // we can safely use [links.at] since all the candidates
      // have been dispatched already
      overload_t const* bound = links.at_overload(sig);

      // call filter
      if(bound)
        filter(*bound);
    }
  }
}

// poll [set] and insert if good match
void max_set::filter(overload_t const& up) {
  signature_t::worse_match is_worse_than;

  max_set_t::iterator
    iter = set.begin()
  , endl = set.end();

  while(iter != endl) {
    overload_t const& e = *iter;

    // [up] is better match, remove [e]
    if( is_worse_than(e.first, up.first) ) {
      iter = set.erase(iter);
      continue;
    }

    // [e] is better match, don't insert [up]
    else if( is_worse_than(up.first, e.first) ) {
      return;
    }

    // poll next overload
    ++iter;
  }

  // none of [set] is better
  set.push_back(up);
}
