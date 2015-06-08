//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_SINGLE_NODE_HPP
#define RTTI_MIXIN_SINGLE_NODE_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"

#include "mmethod/rtti/mixin/mixin_node.hpp"

namespace rtti {

struct single_mixin_node {
public:
  template<bool Declare, typename Mixin, typename Derived>
  struct base
  {};
};

template<typename Mixin, typename Derived>
struct single_mixin_node::base<true, Mixin, Derived>
: private detail::mixin_node_holder
{
  friend detail::mixin_node_holder const&
  rtti_get_mixin(Derived const& d) {
    return static_cast<base const&>(d);
  }
};

} // namespace rtti

#include <boost/typeof/typeof.hpp>
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(rtti::single_mixin_node)

#endif
