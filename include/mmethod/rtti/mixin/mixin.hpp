//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_MIXIN_HPP
#define RTTI_MIXIN_MIXIN_HPP

#include "mmethod/config.hpp"
#include "mmethod/rtti/mixin/mixin_node.hpp"
#include "mmethod/rtti/getter.hpp"

#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>

#include <boost/utility/declval.hpp>

#include <boost/type_traits/add_cv.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace rtti {

template<
  typename Derived
, typename Supers
, typename Declare
, typename MixinNode
>
struct mixin
: public MixinNode::template base<
    Declare::value
  , mixin<Derived, Supers, Declare, MixinNode>
  , Derived
  >
{

private:
  // parent classes manipulation
  typedef typename boost::mpl::transform<
    Supers
  , boost::add_cv<boost::mpl::_>
  >::type parents;

  BOOST_STATIC_CONSTANT(std::size_t, arity = boost::mpl::size<parents>::value);
  struct arity_type { unsigned char __dummy [ 1+arity ]; };
  BOOST_STATIC_ASSERT( sizeof(arity_type) == 1+arity );

  static detail::mixin_node_holder const&
  fetch_node_holder(mixin const& x) {
    Derived const& d = static_cast<Derived const&>(x);
    return rtti_get_mixin(d);
  }

  friend MixinNode rtti_mixin_node_type(Derived const volatile*) {
    return boost::declval<MixinNode>();
  }

public:
  friend struct detail::rtti_getter;
  friend arity_type rtti_parents_size_1p(Derived const volatile*) {
    // dummy body : we don't want any call to this
    return boost::declval<arity_type>();
  }
  template<typename F>
  friend void rtti_parents_foreach(F f, Derived const volatile*) {
    boost::mpl::for_each<
      parents,
      boost::add_pointer<boost::mpl::_>
    >(f);
  }

protected:
  mixin() BOOST_NOEXCEPT_OR_NOTHROW {
    detail::mixin_node_holder const& nh = mixin::fetch_node_holder(*this);
    const_cast<detail::mixin_node_holder&>(nh).rtti_node_value
      = rtti::static_node<Derived>();
  }
  ~mixin() BOOST_NOEXCEPT_OR_NOTHROW {}
};

} // namespace rtti

#endif
