//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[ba_basic
/*`
  Before being used in the __rtti__ framework,
  a class needs to be declared to it.
  This is done using `base_rtti` and `implement_rtti` template classes.

  In order to declare a new class hierarchy,
  each root class /K/ must derive from `base_rtti<` /K/ `>`.
  [ba_foo]

  After each root class has been declared, you can
  add new classes using `implement_rtti` template.
  To extend with class /T/, it must derive
  from `implement_rtti<` /T/, /Supers/ `>` where /Supers/
  is a __sequence__ modelling the inherited classes.
  [important `implement_rtti` must come last in the base class list]
  [ba_bar]

  Then, an id can be retrieved :
  * statically for a class /T/ using `static_id<` /T/ `>()`
  * dynamically for a object /o/ using `get_id(` /o/ `)`
  [ba_use]

  Full information, named a __rtti__ node,
  can also be retrieved the same way,
  with `static_node` and `get_node`
  [ba_node]

  The nodes contain full information about the class hierarchy derivation.
  Each node /n/ contains the following information :
  * the class id, using `rtti_get_id(` /n/ `)`
  * the class number of bases, using `rtti_get_base_arity(` /n/ `)`
  * the class /k/[super th] base's node, using `rtti_get_base_arity(` /n/, /k/ `)`
  [ba_hie]
 */
//]

#include "mmethod/rtti.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace {

//[ba_foo
struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
};
//]

//[ba_bar
using boost::mpl::vector;

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{};

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(basic) {
  //[ba_use
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
  BOOST_CHECK_EQUAL( static_id<lap>(), get_id(l) );
  //]
}

BOOST_AUTO_TEST_CASE(hierarchy) {
  lap l;

  //[ba_node
  rtti_hierarchy h = get_node(l);
  BOOST_CHECK_EQUAL( h, static_node<lap>()       );

  // static_id<> is a shorhand for static_node<>()->id
  BOOST_REQUIRE_EQUAL( rtti_get_id( static_node<foo>() ), static_id<foo>() );
  //]

  //[ba_hie
  // sanity
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 1 );

  // verify hierarchy
  h = rtti_get_base(h, 0);
  BOOST_CHECK_EQUAL( h, static_node<bar>() );
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 1 );

  // second argument to get_base is defaulted to 0
  h = rtti_get_base(h);
  BOOST_CHECK_EQUAL( h, static_node<foo>() );
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 0 );
  //]
}
