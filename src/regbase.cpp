//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/shared/basic.hpp"

#include <boost/config.hpp>
#include <cstdlib>

using namespace rtti::detail;

void rtti::_rtti_bad_dispatch() {
#ifdef BOOST_NO_EXCEPTIONS
  throw bad_dispatch();
#else
  std::abort();
#endif
}

rtti::bad_dispatch::bad_dispatch()
: std::runtime_error("Unresolved call for mmethod") {}
rtti::bad_dispatch::~bad_dispatch() BOOST_NOEXCEPT_OR_NOTHROW {}
