//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_FORWARD_HPP
#define MMETHOD_AMBIGUITY_FORWARD_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"

#include <boost/core/scoped_enum.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace mmethod {
namespace ambiguity {

BOOST_SCOPED_ENUM_DECLARE_BEGIN(action_t)
{
  NOTHING,
  RETRY
}
BOOST_SCOPED_ENUM_DECLARE_END(action_t)

typedef action_t (*ambiguity_handler_t)(size_t, rtti_hierarchy[]);

} // namespace ambiguity
} // namespace mmethod
} // namespace rtti

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
