//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_AMBIGUITY_NULL_HANDLER_HPP
#define BOOST_MMETHOD_AMBIGUITY_NULL_HANDLER_HPP

#include "boost/mmethod/export/table.hpp"

#include "boost/mmethod/ambiguity/noreturn_policy.hpp"

#include <cstdlib>

namespace boost {
namespace mmethod {
namespace ambiguity {

using detail::ambiguity_handler_t;

struct null_policy
: noreturn_policy<null_policy>
{

  BOOST_CONSTEXPR static ambiguity_handler_t
  get_ambiguity_handler() noexcept
  { return NULL; }

  static void bad_dispatch() {
    std::abort();
  }

};

}}} // namespace boost::mmethod::ambiguity

#endif
