//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MIXIN_HPP
#define RTTI_MIXIN_HPP

#include "mmethod/rtti/mixin/mixin.hpp"
#include "mmethod/rtti/getter/getter.hpp"
#include "mmethod/rtti/getter/getter.ipp"

#include <boost/mpl/vector.hpp>

namespace rtti {

//! \brief Base case
template<typename klass>
struct base_rtti
: public mixin<
  klass, boost::mpl::vector<>
, boost::true_type
> {};

//! \brief Any other class
template<typename klass, typename parents>
struct implement_rtti
: public mixin<
  klass, parents
, boost::false_type
> {};

} // namespace rtti

#endif
