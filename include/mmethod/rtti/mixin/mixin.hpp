//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_MIXIN_HPP
#define RTTI_MIXIN_MIXIN_HPP

#include "mmethod/config.hpp"
#include "mmethod/rtti/mixin/mixin_node.hpp"
#include "mmethod/rtti/mixin/implement_traits.hpp"

#include "mmethod/detail/for_each.hpp"

#include <boost/utility/declval.hpp>

namespace rtti {

//! \brief Rtti main class.
//!
//! This struct adds hooks on the current class hierarchy.
//! It mainly does three things:
//! - stores the rtti runtime info as a \c mixin_node_holder.
//!   The details of storage are set by \c MixinNode.
//! - constructs it with the right value
//! - creates ADL-lookup tools to process the class
template<
  typename Derived
, typename Supers
, bool Declare
, typename MixinNode
>
struct mixin
: public MixinNode::template base<
    Declare
  , mixin<Derived, Supers, Declare, MixinNode>
  , Derived
  >
{

private:
  typedef detail::implement_traits<Derived, Supers> itraits;
  typedef typename itraits::parents parents;
  typedef typename itraits::arity_type arity_type;

  //! \brief ADL friends
  //@{
  //! \brief Get the \c MixinNode.
  //! This is there to automatically infer the base-class'
  //! \c MixinNode.
  friend MixinNode rtti_mixin_node_type(Derived const volatile* /*unused*/) {
    return boost::declval<MixinNode>();
  }

  //! \brief Size and iteration over the _parents list
  //! We use size_1p since we cannot create 0-sized types
  friend arity_type rtti_parents_size_1p(Derived const volatile* k)
  { return rtti_parents_size_1p(k); }
  template<typename F>
  friend void rtti_parents_foreach(F f, Derived const volatile* /*unused*/) {
    rtti::mmethod::detail::for_each<parents>(f);
  }
  //@}

protected:
  //! Runtime value manipulation:
  //! The \c mixin_node_holder is stored by \c MixinNode.
  //! The \c MixinNode exports one ADL function called
  //! \c rtti_get_mixin to retrieve the \c mixin_node_holder.
  //! We use that function to set the \c rtti_node
  //! to the right value.
  mixin() BOOST_NOEXCEPT_OR_NOTHROW {
    rtti_node const* const dvalue = rtti::static_node<Derived>();

    typedef detail::mixin_node_holder mnh;
    Derived const& d = static_cast<Derived const&>(*this);
    mnh const& nh = rtti_get_mixin(d);
    const_cast<mnh&>(nh).rtti_node_value = dvalue;
  }
  ~mixin() BOOST_NOEXCEPT_OR_NOTHROW {}
};

} // namespace rtti

#endif
