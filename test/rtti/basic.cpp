//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[ba_basic
/*`
  Before being used in the __rtti__ framework,
  a class needs to be declared to it.
  This is done using `base_rtti` and `implement_rtti` templates.

  [ba_foo]
  [ba_bar]
  [ba_use]
  [ba_exn]
  [ba_node]
  [ba_hie]
 */
//]

#include "mmethod/rtti.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace {

//[ba_foo
/*`
  [heading A new hierarchy]
  In order to declare a new class hierarchy,
  each root class /K/ must derive from [^base_rtti</K/>].
  It creates all the necessary code in order
  to have everything work properly.
 */
class foo
: public base_rtti<foo>
{
public:
  virtual ~foo() {}
};
//]

//[ba_bar
/*`
  [heading Adding classes to the hierarchy]
  After each root class has been declared, you can
  add new classes using `implement_rtti` template.
  To extend with class /T/, it must derive
  from [^implement_rtti</T/, /Bases/>] where /Bases/
  is a __sequence__ listing the inherited classes.
  We will use a `boost::mpl::vector` in the present guide.
 */
using boost::mpl::vector;

class bar
: public foo
, public implement_rtti<bar, vector<foo> >
{};

class baz
: public foo
, public implement_rtti<baz, vector<foo> >
{};

class lap
: public bar
, public implement_rtti<lap, vector<bar> >
{};
/*`
  [important All the classes appearing in /Bases/ must
    be public bases of the class
    and appear before `implement_rtti` in the base class list.

    Base classes that do not appear in /Bases/
    (for example implementation helpers and private bases)
    can come anywhere in the list.
  ]
 */
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_basic) {
  //[ba_use
  /*`
    [heading Runtime identification]
    __rtti__ provides each class an identifier of type
    `rtti_type`. This id can be retrieved :

    * statically for a class /T/ using [^static_id</T/>()]
    * dynamically for a object /obj/ using [^get_id(/obj/)]
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
  BOOST_CHECK_EQUAL( static_id<lap>(), get_id(l) );
  //]

  //[ba_exn
  /*`
    When the passed pointer is NULL,
    the `get_id` function returns
    an identifier evaluating to false.
   */
  foo* fp = NULL;
  BOOST_CHECK( !get_id(fp) );
  //<-
  bar* const bp = NULL;
  BOOST_CHECK( !get_id(bp) );
  //->
  //]
}

BOOST_AUTO_TEST_CASE(test_hierarchy) {
  lap l;

  //[ba_node
  /*`
    [heading Walking the hierarchy at runtime]
    More thorough information can be asked
    from __rtti__ using the nodes.
    Each class is associated a node,
    which can be retrieved either at compile time or at run time.
    It provides an object of type `rtti_hierarchy`,
    containing further information about the hierarchy,
    and can be used for runtime reflection.
    The access primitives are `static_node` and `get_node`.
   */
  rtti_hierarchy h = get_node(l);
  BOOST_CHECK_EQUAL( h, static_node<lap>()       );

  // get_id() is a shorhand for rtti_get_id( get_node() )
  BOOST_CHECK_EQUAL( rtti_get_id(h), get_id(l) );

  // likewise, static_id<> is a shorhand for rtti_get_id( static_node<>() )
  BOOST_CHECK_EQUAL( rtti_get_id(static_node<foo>()), static_id<foo>() );
  //]

  //[ba_hie
  /*`
    The nodes contain full information about the class hierarchy derivation.
    Each node /node/ contains the following information :

    * the class id, using [^rtti_get_id(/node/)]
    * the class number of bases, using [^rtti_get_base_arity(/node/)]
    * the class /k/[super th] base's node, using [^rtti_get_base_arity(/node/, /k/)]
   */
  // sanity
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 1 );

  // verify hierarchy
  h = rtti_get_base(h, 0);
  BOOST_CHECK_EQUAL( h, static_node<bar>() );
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 1 );

  // second argument to get_base is 0 by default
  h = rtti_get_base(h);
  BOOST_CHECK_EQUAL( h, static_node<foo>() );
  BOOST_REQUIRE_EQUAL( rtti_get_base_arity(h), 0 );

  /*`
    [note It is possible to go back and forth between
      the class id and the node value using the functions
      `rtti_get_id` and `rtti_get_node`.
    ]
   */
  //]
}
