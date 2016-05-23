//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/policy/default_policy.hpp"

#include <boost/throw_exception.hpp>

using namespace rtti;
using namespace rtti::mmethod::ambiguity;

void default_policy::bad_dispatch() {
  BOOST_THROW_EXCEPTION( rtti::bad_dispatch() );
}

action_t
default_policy::ambiguity_handler(size_t, rtti_hierarchy const*) {
  return action_t::NOTHING;
}

