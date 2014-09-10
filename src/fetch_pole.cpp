//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/table.hpp"

#include "mmethod/rtti/holder/node.hpp"

using rtti::rtti_type;

using rtti::detail::rtti_node;
using rtti::detail::rtti_hierarchy;

using rtti::hash::detail::hash_map;
using rtti::hash::detail::value_type;

static hash_map::iterator
do_fetch_pole_recursive(
  hash_map const& map
, rtti_hierarchy rt
) {
  // the recursion is unrolled by default
  for(;;) {
    // common case -> we are a pole or a memoized value
    hash_map::iterator it = map.find( rtti_get_id(rt) );
    if(BOOST_LIKELY( !it->empty() ))
      return it;

    // exit condition
    size_t arity = rtti_get_base_arity(rt);

    if(arity == 0)
      return NULL;

    rtti_hierarchy rt0 = rtti_get_base(rt);
    BOOST_ASSERT(rt0);

    // by definition of the poles,
    // a fork either is a pole, or has all its branches behave the same way
    // so we only need to follow the first branch

#ifndef NDEBUG
    // use a recursive version to check for inconsistencies
    if(arity > 1) {
      hash_map::iterator ret = do_fetch_pole_recursive(
        map, rt0
      );

      for(std::size_t k = 1; k < arity; ++k) {
        rtti_hierarchy rtk = rtti_get_base(rt, k);
        hash_map::iterator it = do_fetch_pole_recursive(
          map, rtk
        );

        BOOST_ASSERT(ret == it);
      }

      return ret;
    }
#endif

    // unroll the loop
    rt = rt0;
  }

  return NULL;
}

// can be moved as non-member
value_type
rtti::hash::detail::do_fetch_pole(
  hash_map const& map
, rtti_hierarchy rt0
) BOOST_NOEXCEPT_OR_NOTHROW {

  const rtti_type id0 = rtti_get_id(rt0);

  do {
    // exit condition
    if( rtti_get_base_arity(rt0) == 0)
      break;

    rtti_hierarchy rt = rtti_get_base(rt0);
    BOOST_ASSERT(rt);

    hash_map::iterator ret = do_fetch_pole_recursive(
      map, rt
    );

    if( !ret )
      break;

    BOOST_ASSERT( !ret->empty() );

    // insertion invalidates iterators
    value_type val = ret->value();

    const_cast<hash_map&>(map).insert( id0, val );
#ifdef MMETHOD_USE_DEEP_CACHE
    for(rtti_node const* rt2 = rtti_get_base(rt0); rt2 != rt; rt2 = rtti_get_base(rt2))
      const_cast<hash_map&>(map).insert( rtti_get_id(rt2), val );
#endif

    return val;
  }
  while(false);

  return map.fallback();
}
