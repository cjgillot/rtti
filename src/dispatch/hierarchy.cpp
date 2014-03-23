//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include "mmethod/rtti/holder/node.hpp"

#include "foreach.hpp"

#include <boost/assert.hpp>
#include <vector>
#include <stack>

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
hierarchy_t::do_add(rtti_hierarchy vec) {
  if(!vec)
    return NULL;

  rtti_type id = rtti_get_id(vec);

  { // memorized case
    dict_t::iterator const dit = dict.find(id);
    if(dit != dict.end()) {
      klass_t* k = dit->second;

      BOOST_ASSERT( k->id == id );

      return k;
    }
  }

  // creation case
  std::size_t const arity = rtti_get_base_arity(vec);

  klasses.push_back( new klass_t( id, arity ) );
  klass_t* k = klasses.back();

  dict.insert(std::make_pair( id, k ));
  for(std::size_t i = 0; i < arity; ++i)
    k->bases[i] = do_add( rtti_get_base(vec, i) );

  return k;
}

klass_t const*
hierarchy_t::add(rtti_hierarchy vec) {
  klass_t* base = do_add(vec);
  base->pole = base;
  return base;
}
