//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_IMPLEMENT_HPP
#define RTTI_MMETHOD_IMPLEMENT_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/implement/helper.hpp"

namespace rtti {
namespace dmethod {

template<typename Tag, typename Sig>
struct dmethod_implementation
: detail::make_implement<Tag, typename Tag::template overload<Sig>, Sig>::type {
  typedef typename dmethod_implementation::impl_maker impl_maker;

protected:
  dmethod_implementation() BOOST_NOEXCEPT_OR_NOTHROW {}
};

#define IMPLEMENTATION_MMETHOD(tag, ret, sig)                   \
template<>                                                      \
struct tag::overload<ret sig>                                   \
: ::rtti::dmethod::dmethod_implementation<tag, ret sig> {       \
  static ret call sig;                                          \
  overload() {}                                                 \
};                                                              \
template struct tag::overload<ret sig>;                         \
ret tag::overload<ret sig>                                      \
  ::call sig /* definition here */

#define IMPLEMENT_MMETHOD(name, ret, sig) \
  IMPLEMENTATION_MMETHOD(__MMETHOD_TAG(name), ret, sig)

}} // namespace rtti::mmethod

#endif
