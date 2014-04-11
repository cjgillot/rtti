//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "multifile.hpp"

#include "mmethod/mmethod.hpp"

#include <boost/test/unit_test.hpp>

using namespace test_multifile;

BOOST_AUTO_TEST_CASE(multifile) {
  foo* f = make_foo();
  foo* r = make_bar();
  foo* z = make_baz();

  BOOST_CHECK_EQUAL( f1(*f), 5  );
  BOOST_CHECK_EQUAL( f1(*r), 42 );
  BOOST_CHECK_EQUAL( f1(*z), 10 );

  delete f;
  delete r;
  delete z;
}
