//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "dispatch/hierarchy.hpp"

#include "foreach.hpp"
#include "nodeptr.hpp"

#include <boost/test/unit_test.hpp>

#include <vector>

using namespace rtti;
using namespace rtti_dispatch;

#define NULL_node ((rtti_hierarchy)NULL)

void
order_poles(
  hierarchy_t const& hh
, std::vector<rtti_hierarchy> &npoles
) {
  foreach(klass_t const* k, hh.range()) {
    size_t r = k->get_rank();

    BOOST_CHECK( r < npoles.size() );
    BOOST_CHECK( !npoles[r] );
    npoles[r] = k->get_rtti();
  }

  std::remove(npoles.begin(), npoles.end(), NULL_node);
}

BOOST_AUTO_TEST_CASE(test_compute_poles) {
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

    std::vector<rtti_hierarchy> cs;
    cs.push_back(c0.node);
    cs.push_back(c2.node);
    cs.push_back(c3.node);

    hierarchy_t hier;
    hier.compute_poles(cs);

    std::vector<rtti_hierarchy> npoles ( 5, NULL_node );
    order_poles(hier, npoles);

    BOOST_CHECK_EQUAL(npoles[0], c0.node);
    BOOST_CHECK_EQUAL(npoles[1], c2.node);
    BOOST_CHECK_EQUAL(npoles[2], c3.node);
    BOOST_CHECK_EQUAL(npoles[3], NULL_node);
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
      //     x
      //     |
      //     o
      //    / \
      //   x   x
      //    \ /
      //     o
      //     |
      //     x
      std::vector<rtti_hierarchy> cs;
      cs.push_back(c0.node);
      cs.push_back(c2.node);
      cs.push_back(c3.node);
      cs.push_back(c5.node);

      hierarchy_t hier;
      hier.compute_poles(cs);

      std::vector<rtti_hierarchy> npoles ( 6, NULL_node );
      order_poles(hier, npoles);

      BOOST_CHECK_EQUAL(npoles[0], c0.node);
      BOOST_CHECK((npoles[1] == c2.node && npoles[2] == c3.node)
               || (npoles[2] == c2.node && npoles[1] == c3.node));
      BOOST_CHECK_EQUAL(npoles[3], c4.node);
      BOOST_CHECK_EQUAL(npoles[4], c5.node);
      BOOST_CHECK_EQUAL(npoles[5], NULL_node);
    }

    { // without c4 a pole
      //     x
      //     |
      //     o
      //    / \
      //   o   x
      //    \ /
      //     o
      //     |
      //     x
      std::vector<rtti_hierarchy> cs;
      cs.push_back(c0.node);
      cs.push_back(c3.node);
      cs.push_back(c5.node);

      hierarchy_t hier;
      hier.compute_poles(cs);

      std::vector<rtti_hierarchy> npoles ( 6, NULL_node );
      order_poles(hier, npoles);

      BOOST_CHECK_EQUAL(npoles[0], c0.node);
      BOOST_CHECK_EQUAL(npoles[1], c3.node);
      BOOST_CHECK_EQUAL(npoles[2], c5.node);
      BOOST_CHECK_EQUAL(npoles[3], NULL_node);
    }
  }

  { // multi-diamond inheritance
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

    nodeptr c5 ( 2 );
    c5.node->self.__base[0] = c2.node;
    c5.node->self.__base[1] = c4.node;

    nodeptr c6 ( 1 );
    c6.node->self.__base[0] = c5.node;

    { // with c4 a pole
      //     x
      //     |
      //     o
      //    / \
      //   x   x
      //   |\  |
      //   | \ |
      //   |  \|
      //   \   o
      //    \ /
      //     o
      //     |
      //     x
      std::vector<rtti_hierarchy> cs;
      cs.push_back(c0.node);
      cs.push_back(c2.node);
      cs.push_back(c3.node);
      cs.push_back(c6.node);

      hierarchy_t hier;
      hier.compute_poles(cs);

      std::vector<rtti_hierarchy> npoles ( 7, NULL_node );
      order_poles(hier, npoles);

      BOOST_CHECK_EQUAL(npoles[0], c0.node);
      BOOST_CHECK((npoles[1] == c2.node && npoles[2] == c3.node)
               || (npoles[2] == c2.node && npoles[1] == c3.node));
      BOOST_CHECK_EQUAL(npoles[3], c4.node);
      BOOST_CHECK_EQUAL(npoles[4], c6.node);
      BOOST_CHECK_EQUAL(npoles[5], NULL_node);
    }

    { // without c4 a pole
      //     x
      //     |
      //     o
      //    / \
      //   x   o
      //   |\  |
      //   | \ |
      //   |  \|
      //   \   o
      //    \ /
      //     o
      //     |
      //     x
      // This one should trigger the last case in pseudo_closest:
      // several parent poles but they reduce to only one.
      std::vector<rtti_hierarchy> cs;
      cs.push_back(c0.node);
      cs.push_back(c2.node);
      cs.push_back(c6.node);

      hierarchy_t hier;
      hier.compute_poles(cs);

      std::vector<rtti_hierarchy> npoles ( 7, NULL_node );
      order_poles(hier, npoles);

      BOOST_CHECK_EQUAL(npoles[0], c0.node);
      BOOST_CHECK_EQUAL(npoles[1], c2.node);
      BOOST_CHECK_EQUAL(npoles[2], c6.node);
      BOOST_CHECK_EQUAL(npoles[3], NULL_node);
    }
  }
}
