//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/export/table.hpp"

#include "boost/mmethod/rtti/holder/node.hpp"

#include "foreach.hpp"

using boost::mmethod::rtti_type;

using boost::mmethod::detail::rtti_node;
using boost::mmethod::detail::rtti_hierarchy;

using boost::mmethod::hash_detail::bucket_t;
using boost::mmethod::hash_detail::hash_map;
using boost::mmethod::hash_detail::value_type;

value_type
boost::mmethod::hash_detail::do_fetch_pole(
  hash_map const& map
, rtti_hierarchy rt0
) BOOST_NOEXCEPT_OR_NOTHROW {

  const rtti_type id0 = rtti_get_id(rt0);

  hash_map::iterator it = map.find(id0);
  if(BOOST_LIKELY( !it->empty() ))
    return it->value();

  // search recursively
  foreach_base(rtti_hierarchy base, rt0) {
    value_type ret = do_fetch_pole(map, base);

    // memoize and propagate
    if(ret != map.fallback()) {
      const_cast<hash_map&>(map).insert(id0, ret);
      return ret;
    }
  }

  return map.fallback();
}
