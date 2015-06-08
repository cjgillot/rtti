//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include "foreach.hpp"

// ----- klass ----- //

klass_t::klass_t(
  rtti_hierarchy hh
)
: rtti(hh) {}

klass_t::~klass_t()
{}

//!\brief Compute rank and subtypes bitset
void
klass_t::set_rank(std::size_t r) {
  this->rank = r;

  if(subtype.size() <= r)
    subtype.resize(1+r);

  foreach(klass_t const* b, bases) {
    if(b->subtype.size() <= r)
      const_cast<klass_t*>(b)->subtype.resize(1+r);

    subtype |= b->subtype;
  }

  subtype.set(r);
}
