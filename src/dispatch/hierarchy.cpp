//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include <boost/foreach.hpp>
#include <vector>
#include <stack>

#include "rtti/holder/node.hpp"

// ----- hierarchy ----- //

hierarchy_t::hierarchy_t()
: current_rank(0)
{}

hierarchy_t::~hierarchy_t()
{
  BOOST_FOREACH(klass_t* k, klasses)
    delete k;
}

klass_t*
hierarchy_t::do_add(rtti_hierarchy vec) {
  if(!vec)
    return NULL;

  rtti_type id = rtti_get_id(vec);

  dict_t::iterator dit = dict.find(id);
  if(dit != dict.end()) {
    klass_t* k = dit->second;

    BOOST_ASSERT( k->id == id );

    return k;
  }
  else {
    std::size_t const arity = rtti_get_base_arity(vec);

    klasses.push_back( new klass_t( id, arity ) );
    klass_t& k = *klasses.back();

    dict.insert(std::make_pair( id, &k ));
    for(std::size_t i = 0; i < arity; ++i)
      k.bases[i] = do_add( rtti_get_base(vec, i) );

    return &k;
  }
}

klass_t const*
hierarchy_t::add(rtti_hierarchy vec) {
  klass_t* base = do_add(vec);
  base->pole = base;
  base->is_pole() = true;
  return base;
}
