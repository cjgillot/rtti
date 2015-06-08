//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/table.hpp"

#include "mmethod/rtti/holder/node.hpp"

#include "foreach.hpp"

#include <boost/optional.hpp>

using namespace rtti;

using rtti::rtti_type;

using rtti::detail::rtti_node;
using rtti::detail::rtti_hierarchy;

using rtti::hash::detail::hash_map;
using rtti::hash::detail::value_type;

static boost::optional<value_type>
do_fetch_pole_recursive(
  hash_map const& map
, rtti_hierarchy const rt
) {
  // common case -> we are a pole or a memoized value
  hash_map::iterator it = map.find( rtti_get_id(rt) );
  if(BOOST_LIKELY( !it->empty() ))
    return it->value();

  // search recursively
  foreach_base(rtti_hierarchy base, rt) {
    boost::optional<value_type> ret =
      do_fetch_pole_recursive(map, base);

    // propagate
    if(ret) {
#ifdef MMETHOD_USE_DEEP_CACHE
      const_cast<hash_map&>(map).insert( rtti_get_id(rt), *ret );
#endif

#ifndef NDEBUG
      // check for inconsistency
      foreach_base(rtti_hierarchy base, rt) {
        boost::optional<value_type> r2 =
          do_fetch_pole_recursive(map, base);
        BOOST_ASSERT(!r2 || *r2 == *ret);
      }
#endif

      return ret;
    }
  }

  return boost::none;
}

value_type
rtti::hash::detail::do_fetch_pole(
  hash_map const& map
, rtti_hierarchy rt0
) BOOST_NOEXCEPT_OR_NOTHROW {

  rtti_type const id0 = rtti_get_id(rt0);

  foreach_base(rtti_hierarchy base, rt0) {
    boost::optional<value_type> ret =
      do_fetch_pole_recursive(map, base);

    if(ret) {
      // memoize
#ifndef MMETHOD_USE_DEEP_CACHE
      const_cast<hash_map&>(map).insert( id0, *ret );
#endif

      return *ret;
    }
  }

  return map.fallback();
}
