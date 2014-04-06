//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/export/table.hpp"

#include "boost/mmethod/rtti/holder/node.hpp"

using boost::mmethod::rtti_type;

using boost::mmethod::detail::rtti_node;
using boost::mmethod::detail::rtti_hierarchy;

using boost::mmethod::hash_detail::bucket_t;
using boost::mmethod::hash_detail::hash_map;
using boost::mmethod::hash_detail::value_type;

// can be moved as non-member
value_type
boost::mmethod::hash_detail::do_fetch_pole(
  hash_map const& map
, rtti_hierarchy rt0
, hash_map::iterator it0
) BOOST_NOEXCEPT_OR_NOTHROW {

  const rtti_type id0 = rtti_get_id(rt0);

  rtti_node const* rt = rt0;

  for(;;) {
    // exit condition
    if( rtti_get_base_arity(rt) == 0)
      break;

    // by definition of the poles,
    // a fork either is a pole, or has all its branches behave the same way
    rt = rtti_get_base(rt0);

    BOOST_ASSERT(rt);

    // common case
    hash_map::iterator it = map.find( rtti_get_id(rt) );

    if(BOOST_LIKELY( !it->empty() )) {

      const_cast<hash_map&>(map).insert_at( it0, id0, it->value() );
#ifdef BOOST_MMETHOD_USE_DEEP_CACHE
      for(rtti_node const* rt2 = rtti_get_base(rt0); rt2 != rt; rt2 = rtti_get_base(rt2))
        const_cast<hash_map&>(map).insert( rtti_get_id(rt2), it->value() );
#endif

      return it->value();
    }
  }

  return map.zero()->value();
}
