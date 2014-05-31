//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_AMBIGUITY_NULL_HANDLER_HPP
#define BOOST_MMETHOD_AMBIGUITY_NULL_HANDLER_HPP

#include "boost/mmethod/export/exception.hpp"
#include "boost/mmethod/policy/noreturn_policy.hpp"

#include <cstdlib>
#include <boost/throw_exception.hpp>

namespace boost {
namespace mmethod {
namespace ambiguity {

struct default_policy
: noreturn_policy<default_policy>
{

  static void bad_dispatch() {
    boost::throw_exception( boost::mmethod::bad_dispatch() );
  }

};

}}} // namespace boost::mmethod::ambiguity

#endif
