//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_NODE_HPP
#define RTTI_MIXIN_NODE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rttifwd.hpp"

namespace boost {
namespace mmethod {
namespace mixin_detail {

struct mixin_node_holder {
  rtti_node const* rtti_node_value;
};

#ifdef BOOST_MMETHOD_VIRTUAL_INHERITANCE

struct mixin_node {
  template<bool Declare, typename Mixin, typename Derived>
  struct base
  : private virtual mixin_node_holder
  {
    friend struct mixin_node;

    friend mixin_node_holder const&
    rtti_get_mixin(Derived const& d) {
      Mixin const& m = static_cast<Mixin const&>(d);
      base const& b = static_cast<base const&>(m);
      return b;
    }
  };

  template<typename T>
  static mixin_node_holder const&
  fetch_node_holder(T const& x) {
    return rtti_get_mixin(x);
  }
};

#else

struct mixin_node {
public:
  template<bool Declare, typename Mixin, typename Derived>
  struct base
  {};

  template<typename T>
  static mixin_node_holder const&
  fetch_node_holder(T const& x) {
    return rtti_get_mixin(x);
  }
};

template<typename Mixin, typename Derived>
struct mixin_node::base<true, Mixin, Derived>
: private mixin_node_holder
{
  friend struct mixin_node;

  friend mixin_node_holder const&
  rtti_get_mixin(Derived const& d) {
    return static_cast<base const&>(d);
  }
};

#endif

}}} // namespace boost::mmethod::mixin_detail

#endif
