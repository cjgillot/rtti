//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[un_empty
/*`
  Check the generation code handles
  empty mmethods.
 */
//]

#include "../classes.hpp"

using namespace boost::mmethod;

namespace {

using tags::_v;

BOOST_MMETHOD_DECLARE(empty, int, (_v<foo const&>));

} // namespace <>

BOOST_AUTO_TEST_CASE(test_empty) {
  empty.generate();
}
