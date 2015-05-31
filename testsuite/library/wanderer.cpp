//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "dispatch/forward.hpp"
#include "dispatch/wanderer.hpp"
#include "nodeptr.hpp"

#include <boost/test/unit_test.hpp>

#include <vector>

using namespace rtti;
using namespace rtti_dispatch;

#define NULL_node ((rtti_hierarchy)NULL)

BOOST_AUTO_TEST_CASE(wanderer) {
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

    wanderer_t wanderer;
    wanderer.push_back(c0.node);
    wanderer.push_back(c1.node);
    wanderer.push_back(c2.node);
    wanderer.push_back(c3.node);
    wanderer.push_back(c4.node);

    rtti_hierarchy r = NULL_node;

    r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c0.node);
    r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c1.node);
    r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c2.node);
    r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c3.node);
    r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c4.node);
    r = wanderer.pop(); BOOST_CHECK_EQUAL(r, NULL_node);
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
      wanderer_t wanderer;
      wanderer.push_back(c0.node);
      wanderer.push_back(c1.node);
      wanderer.push_back(c3.node);
      wanderer.push_back(c4.node);

      rtti_hierarchy r = NULL_node;

      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c0.node);
      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c1.node);
      r = wanderer.pop();
      if(r == c2.node) {
        r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c3.node);
      }
      else if(r == c3.node) {
        r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c2.node);
      }
      else {
        BOOST_CHECK(false);
      }
      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c4.node);
      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, NULL_node);
    }

    { // without c3 a pole
      wanderer_t wanderer;
      wanderer.push_back(c0.node);
      wanderer.push_back(c1.node);
      wanderer.push_back(c5.node);

      rtti_hierarchy r = NULL_node;

      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c0.node);
      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c1.node);
      r = wanderer.pop();
      if(r == c2.node) {
        r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c3.node);
      }
      else if(r == c3.node) {
        r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c2.node);
      }
      else {
        BOOST_CHECK(false);
      }
      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c4.node);
      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, c5.node);
      r = wanderer.pop(); BOOST_CHECK_EQUAL(r, NULL_node);
    }
  }
}
