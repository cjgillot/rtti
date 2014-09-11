//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

namespace {

struct foo
: base_rtti<foo>
, private boost::noncopyable {
public:
  virtual ~foo() {}

  int f() const { return 5; }
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{
  int g() const { return 42; }
};

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};

using tags::_v;
BOOST_MMETHOD_DECLARE(f1, int, (long, _v<foo const&>));

BOOST_MMETHOD_IMPLEMENT(f1, int, (long n, foo const& a)) { return n + a.f(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (long n, bar const& a)) { return n + a.g(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (long n, baz const& a)) { return n * a.f(); }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_mixed) {
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( f1(1, f),  6 );
  BOOST_CHECK_EQUAL( f1(2, r), 44 );
  BOOST_CHECK_EQUAL( f1(3, z), 15 );
  BOOST_CHECK_EQUAL( f1(9, l), 51 ); // (lap is-a bar)
}
