//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"
#include "mmethod/rtti/foreign/foreign.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

/*!\example foreign.cpp
 * 
 * This example demonstrates usage of \c rtti for foreign classes.
 * 
 * Four classes are used : \c foo, \c bar, \c baz, \c lap
 * Their ids are then output on stdout
 * 
 * \c foo is the base class
 * \c bar inherits from \c foo
 * \c baz and \c lap respectively inherit from \c foo and \c bar
 */

using namespace boost::mmethod;
using boost::mpl::vector;

namespace {

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

BOOST_MMETHOD_FOREIGN_DECLARE(foo)
BOOST_MMETHOD_FOREIGN_IMPLEMENT(bar, vector<foo>)
BOOST_MMETHOD_FOREIGN_IMPLEMENT(baz, vector<foo>)
BOOST_MMETHOD_FOREIGN_IMPLEMENT(lap, vector<bar>)

} // namespace <>

BOOST_AUTO_TEST_CASE(foreign) {
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
  BOOST_CHECK_EQUAL( static_id<lap>(), get_id(l) );
}
