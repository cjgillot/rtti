//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

/*!\example hierarchy.cpp
 * 
 * This example demonstrates hierarchy traversal with \c rtti.
 * 
 * Three classes are used : \c foo, \c bar, \c baz
 * Their node's addresses and ids are output on stdout.
 * \c baz upward hierarchy is output on stdout.
 * 
 * \c foo is the base class
 * \c bar has a statically defined id \c bar_id
 * \c baz inherits from \c bar
 */

using namespace rtti;
using boost::mpl::vector;

namespace {

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{};

struct baz
: bar
, implement_rtti<baz, vector<bar> >
{};

} // namespace <>

BOOST_AUTO_TEST_CASE(hierarchy) {
  baz z;

  // static_id<> is a shorhand for static_node<>()->id
  BOOST_REQUIRE_EQUAL( rtti_get_id( static_node<foo>() ), static_id<foo>() );

  // sanity
  rtti_hierarchy h = get_node(z);
  BOOST_CHECK_EQUAL( h, static_node<baz>()       );
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 1 );

  // verify hierarchy
  h = rtti_get_base(h, 0);
  BOOST_CHECK_EQUAL( h, static_node<bar>() );
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 1 );

  // second argument to get_base is defaulted to 0
  h = rtti_get_base(h);
  BOOST_CHECK_EQUAL( h, static_node<foo>() );
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 0 );
}
