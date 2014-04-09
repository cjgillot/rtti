//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

/*!\example multiple.cpp
 * 
 * This example demonstrates \c rtti in the case of multiple inheritance.
 * 
 * Four classes are used : \c foo, \c bar, \c baz, \c lap
 * Their ids are then output on stdout
 * 
 * \c foo and \c bar are unrelated base classes
 * \c baz inherits from both \c foo and \c bar
 */

using namespace boost::mmethod;
using boost::mpl::vector;

// BOOST_MMETHOD_VIRTUAL_INHERITANCE is required for this testcase
#ifdef BOOST_MMETHOD_VIRTUAL_INHERITANCE

namespace {

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
};

struct bar
: base_rtti<bar> {
public:
  virtual ~bar() {}
};

struct baz
: foo, bar
, implement_rtti<baz, vector<foo, bar> >
{};

} // namespace

BOOST_AUTO_TEST_CASE(multiple) {
  foo f; bar r; baz z;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
}

#endif
