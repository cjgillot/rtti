//          Copyright Camille Gillot 2012 - 2016.
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
//! This struct should be used at the root of a class hierarchy.
//! The base class should publicly inherit \c base_rtti with itself as first
//! template argument.
//!
//! \code
//!   class MyBase
//!   : public base_rtti<MyBase>
//!   {
//!     // ...
//!   };
//! \endcode
//!
//! \param klass
//!   The class we are defining.
//! \param MixinNode [= single_mixin_node]
//!   The mixin node type we want to use.
//!   By default, \c single_mixin_node only allows single inheritance.
//!   For multiple inheritance, use \c virtual_mixin_node.
template<typename klass, class MixinNode = single_mixin_node>
struct base_rtti
: public mixin<
  klass, boost::mpl::vector<>
, true // Declare
, MixinNode
> {};

namespace detail {

//! \brief Helper to recover parents' mixin node.
template<typename parents>
struct get_mixin_node_type {
private:
  typedef typename boost::mpl::at_c<parents, 0>::type parent0;

  // Recover type thanks to ADL-found rtti_mixin_node_type.
  BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested,
    rtti_mixin_node_type(
      (parent0*)NULL
    )
  )

public:
  typedef typename nested::type type;
};

} // namespace detail

//! \brief Any other class
//! This struct should be used for any rtti-enabled class
//! that is not the root of a hierarchy.
//! The class should publicly inherit \c implement_rtti with itself as first
//! template argument.
//!
//! \code
//!   class MyClass
//!   : public MyBase
//!   , public implement_rtti<MyClass, boost::mpl::vector<MyBase> >
//!   {
//!     // ...
//!   };
//! \endcode
//!
//! \param klass
//!   The class we are defining.
//! \param parents
//!   An MPL sequence of the rtti classes we are inheriting from.
template<typename klass, typename parents>
struct implement_rtti
: public mixin<
  klass, parents
, false // Declare
, typename detail::get_mixin_node_type<parents>::type
> {};

} // namespace rtti

#endif
