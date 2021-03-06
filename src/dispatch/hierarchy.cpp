//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include "foreach.hpp"
#include "util.hpp"

// ----- hierarchy ----- //

hierarchy_t::hierarchy_t()
: current_rank(0)
{}

hierarchy_t::~hierarchy_t()
{
  foreach(klass_t* k, klasses) {
    delete k;
  }
}

klass_t const*
hierarchy_t::try_fetch(rtti_hierarchy hh) const {
  poles_map_t::const_iterator it = poles.find(hh);
  if( it == poles.end() ) {
    return NULL;
  }

  klass_t const* k = it->second;
  BOOST_ASSERT(
    std::find(klasses.begin(), klasses.end(), k)
      != klasses.end()
  );
  return k;
}

klass_t const*
hierarchy_t::fetch(rtti_hierarchy hh) const {
  klass_t const* ret = try_fetch(hh);
  BOOST_ASSERT(ret);
  return ret;
}

klass_t const*
hierarchy_t::try_fetch_from(rtti_hierarchy hh) const {
  klass_t const* ret = try_fetch(hh);

  if(ret) {
    return ret;
  }

  // upcast to find something
  foreach_base(rtti_hierarchy bb, hh) {
    klass_t const* pole = try_fetch_from(bb);

    if(pole && ret) {
      BOOST_ASSERT(pole == ret);
    }
    else if(!ret) {
      ret = pole;
    }
  }

  return ret;
}

klass_t const*
hierarchy_t::fetch_from(rtti_hierarchy hh) const {
  klass_t const* ret = try_fetch_from(hh);
  BOOST_ASSERT(ret);
  return ret;
}
