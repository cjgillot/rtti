//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "./classes.hpp"

#include <boost/test/unit_test.hpp>

#include <iostream>

using namespace rtti;

namespace {

using boost::mpl::vector;

struct first_policy
: public mmethod::default_policy
{
  static void ambiguity_handler(std::size_t, rtti_hierarchy*);
};

void
first_policy::ambiguity_handler(std::size_t n, rtti_hierarchy* types) {
  BOOST_CHECK_EQUAL(n, 2u);

  if(rtti_get_base_arity(types[0])) {
    types[0] = rtti_get_base(types[0]);
    BOOST_THROW_EXCEPTION(rtti::retry_dispatch());
  }
  else if(rtti_get_base_arity(types[1])) {
    types[1] = rtti_get_base(types[1]);
    BOOST_THROW_EXCEPTION(rtti::retry_dispatch());
  }
  else
    return;
}

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};

using tags::_v;
DECLARE_MMETHOD_POLICY(first, int, (_v<foo&>, _v<foo&>), first_policy);

IMPLEMENT_MMETHOD(first, int, (foo&, foo&)) { return  0; }
IMPLEMENT_MMETHOD(first, int, (lap&, foo&)) { return 13; }
IMPLEMENT_MMETHOD(first, int, (foo&, lap&)) { return 21; }

// lap-lap is not defined, but the policy resolves it to bar-lap

} // namespace <>

BOOST_AUTO_TEST_CASE(test_first) {
  foo a; bar b; lap c;
  foo x; bar y; lap z;

  BOOST_CHECK_EQUAL( first(a, x),  0 ); // (1-1 case)
  BOOST_CHECK_EQUAL( first(a, y),  0 ); // (1-2 case)
  BOOST_CHECK_EQUAL( first(a, z), 21 ); // (1-3 case)
  BOOST_CHECK_EQUAL( first(b, x),  0 ); // (2-1 case)
  BOOST_CHECK_EQUAL( first(b, y),  0 ); // (2-2 case)
  BOOST_CHECK_EQUAL( first(b, z), 21 ); // (2-3 case)
  BOOST_CHECK_EQUAL( first(c, x), 13 ); // (3-1 case)
  BOOST_CHECK_EQUAL( first(c, y), 13 ); // (3-2 case)
  BOOST_CHECK_EQUAL( first(c, z), 21 ); // (3-3 case)
}
