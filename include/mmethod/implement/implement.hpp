//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_IMPLEMENT_HPP
#define BOOST_MMETHOD_IMPLEMENT_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/implement/helper.hpp"

namespace boost {
namespace mmethod {

template<typename Tag, typename Sig>
struct mmethod_implementation
: detail::make_implement<Tag, typename Tag::template overload<Sig>, Sig>::type {
  typedef typename mmethod_implementation::impl_maker impl_maker;

protected:
  mmethod_implementation() BOOST_NOEXCEPT_OR_NOTHROW {}
};

#define BOOST_MMETHOD_IMPLEMENTATION(tag, ret, sig)             \
template<>                                                      \
struct tag::overload<ret sig>                                   \
: ::boost::mmethod::mmethod_implementation<tag, ret sig> {      \
  static ret call sig;                                          \
  overload() {}                                                 \
};                                                              \
ret tag::overload<ret sig>                                      \
  ::call sig /* definition here */

#define BOOST_MMETHOD_IMPLEMENT(name, ret, sig) \
  BOOST_MMETHOD_IMPLEMENTATION(BOOST_MMETHOD_TAG(name), ret, sig)

}} // namespace boost::mmethod

#endif
