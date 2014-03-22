//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_BASIC_HPP
#define RTTI_MMETHOD_BASIC_HPP

#include <stdexcept>
#include <stdint.h>

#include "mmethod/rttifwd.hpp"

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {

typedef void(*invoker_t)();
BOOST_STATIC_ASSERT_MSG( sizeof(invoker_t) <= sizeof(uintptr_t), "Platform not supported" );

struct bad_dispatch
: std::runtime_error {
  bad_dispatch();
  virtual ~bad_dispatch() BOOST_NOEXCEPT_OR_NOTHROW;
};

void _rtti_bad_dispatch();

} // namespace rtti

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
