//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_BAD_DISPATCH_HPP
#define BOOST_MMETHOD_BAD_DISPATCH_HPP

#include "boost/mmethod/config.hpp"

#include <exception>
#include <stdexcept>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace mmethod {

struct bad_rtti
: std::runtime_error {
  bad_rtti();
  virtual ~bad_rtti() BOOST_NOEXCEPT_OR_NOTHROW;
};

struct bad_dispatch
: std::runtime_error {
  bad_dispatch();
  virtual ~bad_dispatch() BOOST_NOEXCEPT_OR_NOTHROW;
};

}} // namespace boost::mmethod

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
