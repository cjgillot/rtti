//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

#include <boost/test/unit_test.hpp>

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

using namespace rtti;

/*!\example rvreferences.cpp
 *
 * Test whether we can handle rvalue references.
 */

namespace {

using tags::_v;
DECLARE_MMETHOD(rvref, int, (_v<foo&&>));

IMPLEMENT_MMETHOD(rvref, int, (foo&&)) { return 5; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_rvreferences) {
  foo f; bar r;

  BOOST_CHECK_EQUAL( rvref(std::move(f)), 5  );
  BOOST_CHECK_EQUAL( rvref(std::move(r)), 5  );
}

#endif
