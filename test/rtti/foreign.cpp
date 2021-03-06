//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[fo_foreign
/*`
  If you cannot afford to alter the class definition to
  declare the __rtti__ hierarchy, the foreign declaration
  mechanism allows to import existing classes.

  For example, the non-modifiable class structure
  may come from a class hierarchy in another library.

  This feature requires the standard C++ RTTI `typeid`.
  There is a project to port this feature to use __typeindex__.

  [fo_decl]
  [fo_import_decl]
  [fo_import_impl]
  [fo_use]
 */
//]

#include "mmethod/rtti.hpp"
#include "mmethod/rtti/foreign/foreign.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;
using boost::mpl::vector;

namespace {

//[fo_decl
/*`
  Suppose you have the following structure.
 */
struct foo {
public:
  virtual ~foo() {}
};

struct bar
: foo
{};

struct baz
: foo
{};

struct lap
: bar
{};
//]

//[fo_import_decl
/*`
  Given a class hierarchy rooted at class /K/,
  you can declare it to use __rtti__ using
  [^MMETHOD_FOREIGN_DECLARE(/K/)] macro.
  It will generate a repository of class types.
 */
MMETHOD_FOREIGN_DECLARE(foo)
//]
//[fo_import_impl
/*`
  Then, for each class /K/, with bases /Supers/,
  you can declare it using [^MMETHOD_FOREIGN_IMPLEMENT(/K/, /Supers/)].
  /Supers/ is a __sequence__,
  defined the same way than for `implement_rtti`.
 */
MMETHOD_FOREIGN_IMPLEMENT(bar, vector<foo>)
MMETHOD_FOREIGN_IMPLEMENT(baz, vector<foo>)
MMETHOD_FOREIGN_IMPLEMENT(lap, vector<bar>)
/*`
  [note
    This mechanism does not support merging hierarchies
    declared using different MMETHOD_FOREIGN_DECLARE calls.
  ]
 */
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_foreign) {
  //[fo_use
  /*`
    Then, the imported class hierarchy works
     as-if it were a native __rtti__ hierarchy.
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
  BOOST_CHECK_EQUAL( static_id<lap>(), get_id(l) );
  /*`
    [note
      Performance however is different.
      The foreign mechanism relies on associative container lookup,
      while the native behaviour just makes one memory access
      to get the node.
    ]
   */
  //]
}
