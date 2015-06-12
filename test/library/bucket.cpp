//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/hash_map.hpp"

#include <boost/test/unit_test.hpp>

using namespace rtti;
using namespace rtti::hash::detail;

#define EVEN_MASK (~(uintptr_t(1)))

BOOST_AUTO_TEST_CASE(test_bucket) {
  BOOST_STATIC_ASSERT( sizeof(value_type) >= sizeof(void(*)()) );

  bucket_t bucket;
  key_type   key = reinterpret_cast<key_type>( rand() & EVEN_MASK );
  value_type val = static_cast<value_type>( rand() );

  BOOST_CHECK( bucket.empty() );

  bucket.set(key, val);

  BOOST_CHECK( !bucket.empty() );
  BOOST_CHECK_EQUAL( bucket.key(),   key );
  BOOST_CHECK_EQUAL( bucket.value(), val );

  bucket.reset();

  BOOST_CHECK( bucket.empty() );
}
