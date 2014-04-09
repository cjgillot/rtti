//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

namespace {

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
  
  int f() { return 5; }
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{
  int g() { return 42; }
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
BOOST_MMETHOD_DECLARATION(f1_t, int, (_v<foo&>));

BOOST_MMETHOD_IMPLEMENTATION(f1_t, int, (foo& a)) { return a.f(); }
BOOST_MMETHOD_IMPLEMENTATION(f1_t, int, (bar& a)) { return a.g(); }
BOOST_MMETHOD_IMPLEMENTATION(f1_t, int, (baz& a)) { return 2 * a.f(); }

} // namespace <>

BOOST_AUTO_TEST_CASE(prefetch) {
  f1_t f1;

  lap l;

  typedef f1_t::function_type func_t;
  func_t fp = f1.fetch(l);

  BOOST_CHECK_EQUAL( fp(l), 42 );
}
