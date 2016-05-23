//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_VIRTUAL_NODE_HPP
#define RTTI_MIXIN_VIRTUAL_NODE_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"

#include "mmethod/rtti/mixin/mixin_node.hpp"

namespace rtti {

struct virtual_mixin_node {
  template<bool Declare, typename Mixin, typename Derived>
  struct base
  : private virtual detail::mixin_node_holder
  {
    friend struct virtual_mixin_node;

    friend detail::mixin_node_holder const&
    rtti_get_mixin(Derived const& d) {
      Mixin const& m = static_cast<Mixin const&>(d);
      base const& b = static_cast<base const&>(m);
      return b;
    }
  };
};

} // namespace rtti

#include <boost/typeof/typeof.hpp>
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(rtti::virtual_mixin_node)

#endif
