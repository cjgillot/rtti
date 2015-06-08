//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_HPP
#define RTTI_MIXIN_HPP

#include "mmethod/config.hpp"
#include "mmethod/rtti/mixin/mixin.hpp"

#include "mmethod/rtti/mixin/single_node.hpp"
#include "mmethod/rtti/mixin/virtual_node.hpp"

#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>

namespace rtti {

//! \brief Base case
template<typename klass, class MixinNode = single_mixin_node>
struct base_rtti
: public mixin<
  klass, boost::mpl::vector<>
, boost::true_type
, MixinNode
> {};

//! \brief Any other class
template<typename klass, typename parents>
struct implement_rtti
: public mixin<
  klass, parents
, boost::false_type
, BOOST_TYPEOF_TPL(rtti_mixin_node_type(
      (typename boost::mpl::at_c<parents, 0>::type*)NULL
  ))
> {};

} // namespace rtti

#endif
