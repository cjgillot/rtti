//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_RTTI_MIXIN_NODE_HPP
#define BOOST_MMETHOD_RTTI_MIXIN_NODE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rttifwd.hpp"

namespace boost {
namespace mmethod {
namespace mixin_detail {

struct mixin_node_holder {
  rtti_node const* rtti_node_value;
};

}}} // namespace boost::mmethod::mixin_detail

#endif
