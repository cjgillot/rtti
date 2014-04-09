//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/exception.hpp"

#include "early.hpp"

#include <cstdlib>

rtti::bad_rtti::bad_rtti()
: std::runtime_error("Invalid pointer passed to rtti::get_node") {}
rtti::bad_rtti::~bad_rtti() BOOST_NOEXCEPT_OR_NOTHROW {}

rtti::bad_dispatch::bad_dispatch()
: std::runtime_error("Unresolved call for mmethod") {}
rtti::bad_dispatch::~bad_dispatch() BOOST_NOEXCEPT_OR_NOTHROW {}
