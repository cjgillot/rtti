//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;
using boost::mpl::vector;

/*!\example diamond.cpp
 *
 * This example demonstrate mmethods with multiple inheritance.
 *
 * Four classes are used : \c foo, \c bar, \c baz, \c lap
 * Their ids are then output on stdout
 *
 * \c foo is the base class
 * \c bar has a statically defined id \c bar_id
 * \c baz and \c lap respectively inherit from \c foo and \c bar
 */

namespace {

struct foo1
: base_rtti<foo1, virtual_mixin_node> {
public:
  virtual ~foo1() {}
};

struct foo2
: base_rtti<foo2, virtual_mixin_node> {
public:
  virtual ~foo2() {}
};

struct bar
: virtual foo1
, implement_rtti<bar, vector<foo1> >
{};

struct baz
: virtual foo1, foo2
, implement_rtti<baz, vector<foo1, foo2> >
{};

struct lap
: bar, baz
, implement_rtti<lap, vector<bar, baz> >
{};

using tags::_v;
DECLARE_MMETHOD(f1, int, (_v<foo1&>));

IMPLEMENT_MMETHOD(f1, int, (foo1&)) { return 5; }
IMPLEMENT_MMETHOD(f1, int, (bar &)) { return 7; }
IMPLEMENT_MMETHOD(f1, int, (lap &)) { return 15; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_diamond) {
  foo1 f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( f1(f), 5  );
  BOOST_CHECK_EQUAL( f1(r), 7  );
  BOOST_CHECK_EQUAL( f1(z), 5  );
  BOOST_CHECK_EQUAL( f1(l), 15 );
}
