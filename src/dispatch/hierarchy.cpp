//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include "mmethod/rtti/holder/node.hpp"

#include "foreach.hpp"
#include "util.hpp"

#include <vector>

// ASSERT macro
#define FIND_KNOWN(k) \
    (std::find(klasses.begin(), klasses.end(), k))
using namespace rtti_dispatch;

// ----- hierarchy ----- //

hierarchy_t::hierarchy_t()
: current_rank(0)
{}

hierarchy_t::~hierarchy_t()
{
  foreach(klass_t* k, klasses)
    delete k;
}

klass_t*
hierarchy_t::add(rtti_hierarchy vec) {
  BOOST_ASSERT(vec);

  { // memorized case
    poles_map_t::const_iterator const dit = poles.find(vec);
    if(dit != poles.end()) {
      klass_t* k = dit->second;

      BOOST_ASSERT( k->rtti == vec );

      return k;
    }
  }

  // creation case
  std::size_t const arity = rtti_get_base_arity(vec);

  klasses.push_back( new klass_t( vec ) );
  klass_t* k = klasses.back();

  // fill base classes
  k->bases.reserve(arity);
  for(std::size_t i = 0; i < arity; ++i) {
    rtti_hierarchy base = rtti_get_base(vec, i);

    poles_map_t::const_iterator it = poles.find(base);
    if(it != poles.end())
      k->bases.push_back(it->second);

    make_unique_sort(k->bases);
  }

  poles.insert(std::make_pair(vec, k));

  return k;
}

void
hierarchy_t::remove(klass_t const* k, klass_t const* replace) {
  klasses.erase(std::remove(klasses.begin(), klasses.end(), k), klasses.end());

  poles_map_t::iterator dit = poles.find(k->get_rtti());
  BOOST_ASSERT(dit != poles.end());
  if(replace) {
    BOOST_ASSERT( FIND_KNOWN(replace) != klasses.end() );
    dit->second = const_cast<klass_t*>(replace);
  }
  else {
    poles.erase(dit);
  }

#ifndef NDEBUG
  // verify k has not been referenced elsewhere
  typedef std::pair<rtti_hierarchy, klass_t*> pair_t;
  foreach(pair_t const& p, poles) {
    BOOST_ASSERT(p.second != k);
  }
#endif

  delete k;
}

//!\brief Compute rank and subtypes bitset
void
hierarchy_t::pole_init(klass_t* k) {
  BOOST_ASSERT( FIND_KNOWN(k) != klasses.end() );
  BOOST_ASSERT( poles.find(k->get_rtti()) != poles.end() );

  std::size_t r = current_rank++;
  k->set_rank(r);
}


klass_t const*
hierarchy_t::fetch(rtti_hierarchy hh) const {
  BOOST_ASSERT( poles.find(hh) != poles.end() );
  klass_t const* k = poles.at(hh);
  BOOST_ASSERT( FIND_KNOWN(k) != klasses.end() );
  return k;
}
