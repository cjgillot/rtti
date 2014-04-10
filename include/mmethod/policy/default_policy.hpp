//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_NULL_HANDLER_HPP
#define MMETHOD_AMBIGUITY_NULL_HANDLER_HPP

#include "mmethod/export/exception.hpp"
#include "mmethod/policy/noreturn_policy.hpp"

#include <cstdlib>

namespace rtti {
namespace mmethod {
namespace ambiguity {

struct default_policy
: noreturn_policy<default_policy>
{

  static void bad_dispatch() {
#ifndef BOOST_NO_EXCEPTIONS
    throw rtti::bad_dispatch();
#else
    std::abort();
#endif
  }

};

}}} // namespace rtti::mmethod::ambiguity

#endif
