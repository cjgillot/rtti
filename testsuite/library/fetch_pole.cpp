//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/table.hpp"
#include "nodeptr.hpp"

#include <boost/test/unit_test.hpp>

#include <vector>
#include <set>

using namespace rtti;
namespace hd = hash::detail;

using namespace detail;

BOOST_AUTO_TEST_CASE(fetch_pole) {
  { // linear inheritance
    nodeptr c0 ( 0 );

    nodeptr c1 ( 1 );
    c1.node->self.__base[0] = c0.node;

    nodeptr c2 ( 1 );
    c2.node->self.__base[0] = c1.node;

    nodeptr c3 ( 1 );
    c3.node->self.__base[0] = c2.node;

    nodeptr c4 ( 1 );
    c4.node->self.__base[0] = c3.node;

    hd::hash_map map;
    map.create(3);
    map.insert(rtti_get_id(c0.node), 0);
    map.insert(rtti_get_id(c2.node), 2);
    map.insert(rtti_get_id(c4.node), 4);

    BOOST_CHECK_EQUAL(hd::fetch_pole(map, c4.node), 4);
    BOOST_CHECK_EQUAL(hd::fetch_pole(map, c3.node), 2);
    BOOST_CHECK_EQUAL(hd::fetch_pole(map, c2.node), 2);
    BOOST_CHECK_EQUAL(hd::fetch_pole(map, c1.node), 0);
    BOOST_CHECK_EQUAL(hd::fetch_pole(map, c0.node), 0);
  }

  { // diamond inheritance
    nodeptr c0 ( 0 );

    nodeptr c1 ( 1 );
    c1.node->self.__base[0] = c0.node;

    nodeptr c2 ( 1 );
    c2.node->self.__base[0] = c1.node;

    nodeptr c3 ( 1 );
    c3.node->self.__base[0] = c1.node;

    nodeptr c4 ( 2 );
    c4.node->self.__base[0] = c2.node;
    c4.node->self.__base[1] = c3.node;

    nodeptr c5 ( 1 );
    c5.node->self.__base[0] = c4.node;

    { // with c4 a pole
      hd::hash_map map;
      map.create(3);
      map.insert(rtti_get_id(c0.node), 0);
      map.insert(rtti_get_id(c3.node), 3);
      map.insert(rtti_get_id(c4.node), 4);

      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c5.node), 4);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c4.node), 4);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c3.node), 3);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c2.node), 0);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c1.node), 0);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c0.node), 0);
    }

    { // without c4 a pole
      hd::hash_map map;
      map.create(3);
      map.insert(rtti_get_id(c0.node), 0);
      map.insert(rtti_get_id(c1.node), 1);
      map.insert(rtti_get_id(c5.node), 5);

      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c5.node), 5);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c4.node), 1);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c3.node), 1);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c2.node), 1);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c1.node), 1);
      BOOST_CHECK_EQUAL(hd::fetch_pole(map, c0.node), 0);
    }
  }
}

