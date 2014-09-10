//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_HASH_FETCH_POLE_HPP
#define BOOST_MMETHOD_HASH_FETCH_POLE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/export/table/table.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace mmethod {
namespace hash_detail {

// is to be invoked without a mutex
extern value_type
do_fetch_pole(
  hash_map const&
, rtti_hierarchy rt0
) BOOST_NOEXCEPT_OR_NOTHROW;

inline value_type
BOOST_MMETHOD_ATTRIBUTE_PURE
fetch_pole(
  hash_map const& map
, rtti_hierarchy rt
) BOOST_NOEXCEPT_OR_NOTHROW {

  const rtti_type id0 = rtti_get_id(rt);

  hash_map::iterator const it0 = map.find(id0);

  if(BOOST_LIKELY( it0->key() == id0 ))
    return it0->value();

  return do_fetch_pole(map, rt);
}

}}} // namespace boost::mmethod::holder_detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
