//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

/*!\example basic.cpp
 * 
 * This example demonstrates basic usage of \c rtti.
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
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};

} // namespace <>

BOOST_AUTO_TEST_CASE(basic) {
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
  BOOST_CHECK_EQUAL( static_id<lap>(), get_id(l) );
}
