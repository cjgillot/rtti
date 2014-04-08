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
DECLARE_MMETHOD(f1, int, (_v<foo const&>));

IMPLEMENT_MMETHOD(f1, int, (foo const& a)) { return a.f(); }
IMPLEMENT_MMETHOD(f1, int, (bar const& a)) { return a.g(); }
IMPLEMENT_MMETHOD(f1, int, (baz const& a)) { return 2 * a.f(); }

} // namespace <>

BOOST_AUTO_TEST_CASE(unary) {
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( f1(f),  5 );
  BOOST_CHECK_EQUAL( f1(r), 42 );
  BOOST_CHECK_EQUAL( f1(z), 10 );
  BOOST_CHECK_EQUAL( f1(l), 42 ); // (lap is-a bar)
}
