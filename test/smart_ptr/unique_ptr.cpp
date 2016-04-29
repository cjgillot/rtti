//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"

#include <boost/test/unit_test.hpp>

using namespace rtti::mmethod;

#ifndef BOOST_NO_CXX11_SMART_PTR

#include <memory>
#include "mmethod/smart_ptr/stl_unique_ptr.hpp"

namespace {

using tags::_v;
DECLARE_MMETHOD(unique, int, (_v<std::unique_ptr<foo>>));

IMPLEMENT_MMETHOD(unique, int, (std::unique_ptr<foo> a)) { return a->f(); }
IMPLEMENT_MMETHOD(unique, int, (std::unique_ptr<bar> a)) { return a->g(); }
IMPLEMENT_MMETHOD(unique, int, (std::unique_ptr<baz> a)) { return 2 * a->f(); }
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_unique_ptr) {
  typedef std::unique_ptr<foo> pointer;
  pointer f { new foo };
  pointer r { new bar };
  pointer z { new baz };
  pointer l { new lap };

  BOOST_CHECK_EQUAL( unique(std::move(f)),  5 );
  BOOST_CHECK_EQUAL( unique(std::move(r)), 42 );
  BOOST_CHECK_EQUAL( unique(std::move(z)), 10 );
  BOOST_CHECK_EQUAL( unique(std::move(l)), 42 ); // (lap is-a bar)
}

#endif
