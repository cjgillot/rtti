//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/hash_map.hpp"

#include <boost/test/unit_test.hpp>
#include <map>

using namespace rtti;
using namespace rtti::hash::detail;

#define EVEN_MASK (~(uintptr_t(1)))

BOOST_AUTO_TEST_CASE(test_hash_map) {
  typedef hash_map::iterator iter_t;

  hash_map map;

  size_t initsz = rand() % (1<<20);
  map.create( initsz );

  // check sufficient size
  BOOST_CHECK( map.size() >= initsz );
  // check power of 2
  BOOST_CHECK_EQUAL( map.size() & (map.size()-1), 0 );

  // {{{ create sample
  typedef std::map<key_type, value_type> pop_t;
  typedef pop_t::const_iterator popit_t;
  typedef pop_t::value_type pair_t;
  pop_t pop;

  for(size_t i = 0; i < initsz; ++i) {
    key_type k = reinterpret_cast<key_type>( rand() & EVEN_MASK);
    value_type v = static_cast<value_type>( rand() );

    pop.insert(std::make_pair(k,v));
  }
  // }}}

  for(popit_t i = pop.begin(), e = pop.end(); i != e; ++i) {
    pair_t const& p = *i;
    map.insert(p.first, p.second);
  }

  for(popit_t i = pop.begin(), e = pop.end(); i != e; ++i) {
    pair_t const& p = *i;
    iter_t it = map.find(p.first);

    BOOST_CHECK( !it->empty() );
    BOOST_CHECK_EQUAL( it->key(),   p.first  );
    BOOST_CHECK_EQUAL( it->value(), p.second );
  }
}
