//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_BAD_DISPATCH_HPP
#define RTTI_MMETHOD_BAD_DISPATCH_HPP

#include "mmethod/config.hpp"

#include <exception>
#include <stdexcept>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {

struct bad_dispatch
: std::runtime_error {
  bad_dispatch();
  virtual ~bad_dispatch() BOOST_NOEXCEPT_OR_NOTHROW;
};

} // namespace rtti

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
