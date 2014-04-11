//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_RTTI_MIXIN_SINGLE_NODE_HPP
#define BOOST_MMETHOD_RTTI_MIXIN_SINGLE_NODE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rttifwd.hpp"

#include "boost/mmethod/rtti/mixin/mixin_node.hpp"

namespace boost {
namespace mmethod {

struct single_mixin_node {
public:
  template<bool Declare, typename Mixin, typename Derived>
  struct base
  {};
};

template<typename Mixin, typename Derived>
struct single_mixin_node::base<true, Mixin, Derived>
: private mixin_detail::mixin_node_holder
{
  friend mixin_detail::mixin_node_holder const&
  rtti_get_mixin(Derived const& d) {
    return static_cast<base const&>(d);
  }
};

}} // namespace boost::mmethod

#include <boost/typeof/typeof.hpp>
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::mmethod::single_mixin_node)

#endif
