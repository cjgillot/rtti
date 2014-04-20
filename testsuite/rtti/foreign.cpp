//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[fo_foreign
/*`
  If you cannot afford to alter the class definition to
  declare the __rtti__ hierarchy, the foreign declaration
  mechanism allows to import existing classes.
  
  Suppose you have the following structure
  [fo_decl]
  
  Given a class hierarchy rooted at class /K/,
  you can declare it to use __rtti__ using
  `MMETHOD_FOREIGN_DECLARE(`/K/`)` macro.
  It will generate a repository of class types.
  [fo_import_decl]
  
  Then, for each class /K/, with bases /Supers/,
  you can declare it using `MMETHOD_FOREIGN_IMPLEMENT(`/K/,/Supers/`)`.
  [fo_import_impl]
  
  [note
    This mechanism does not support merging hierarchies
    declared using different MMETHOD_FOREIGN_DECLARE calls.
  ]
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
MMETHOD_FOREIGN_DECLARE(foo)
//]
//[fo_import_impl
MMETHOD_FOREIGN_IMPLEMENT(bar, vector<foo>)
MMETHOD_FOREIGN_IMPLEMENT(baz, vector<foo>)
MMETHOD_FOREIGN_IMPLEMENT(lap, vector<bar>)
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(foreign) {
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
  BOOST_CHECK_EQUAL( static_id<lap>(), get_id(l) );
}
