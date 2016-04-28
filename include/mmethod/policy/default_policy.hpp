//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_NULL_HANDLER_HPP
#define MMETHOD_AMBIGUITY_NULL_HANDLER_HPP

#include "mmethod/export/exception.hpp"
#include "mmethod/policy/duplicator.hpp"

#include <boost/shared_ptr.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace mmethod {
namespace ambiguity {

struct default_policy {
  static void bad_dispatch();
  static void ambiguity_handler(size_t /*arity*/,
                                rtti_hierarchy const* /*signature*/);

  static boost::shared_ptr<duplicator> make_duplicate();
};

} // namespace ambiguity

using ambiguity::default_policy;

} // namespace mmethod
} // namespace rtti

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
