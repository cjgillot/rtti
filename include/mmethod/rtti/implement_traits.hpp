//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_IMPLEMENT_TRAITS_HPP
#define BOOST_MMETHOD_IMPLEMENT_TRAITS_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rtti/mixin/mixin_node.hpp"
#include "boost/mmethod/rtti/foreign/type_index.hpp"

#include <boost/mpl/size.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/add_cv.hpp>

namespace boost {
namespace mmethod {
namespace detail {

template<typename Klass, typename Parents>
struct implement_traits {
  // parent classes manipulation
  typedef typename boost::mpl::transform<
    Parents
  , boost::add_cv<boost::mpl::_>
  >::type parents;

  BOOST_STATIC_CONSTANT(std::size_t, arity = boost::mpl::size<parents>::value);
  struct arity_type { unsigned char __dummy [ 1+arity ]; };
  BOOST_STATIC_ASSERT( sizeof(arity_type) == 1+arity );
};

}}} // namespace boost::mmethod::detail

#endif
