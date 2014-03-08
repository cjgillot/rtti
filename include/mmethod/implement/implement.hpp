#ifndef RTTI_DMETHOD_IMPLEMENT_HPP
#define RTTI_DMETHOD_IMPLEMENT_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/implement/helper.hpp"

namespace rtti {
namespace dmethod {

template<typename Tag, typename Sig>
struct dmethod_implementation
: detail::make_implement<Tag, typename Tag::template overload<Sig>, Sig>::type {
  typedef typename dmethod_implementation::impl_maker impl_maker;

protected:
  dmethod_implementation() noexcept {}
};

#define IMPLEMENTATION_MMETHOD(tag, ret, sig)                   \
template<>                                                      \
struct tag::overload<ret sig>                                   \
: ::rtti::dmethod::dmethod_implementation<tag, ret sig> {       \
  static ret call sig;                                          \
  overload() {}                                                 \
};                                                              \
ret tag::overload<ret sig>              \
  ::call sig /* definition here */

#define IMPLEMENT_MMETHOD(name, ret, sig) \
  IMPLEMENTATION_MMETHOD(__MMETHOD_TAG(name), ret, sig)

}} // namespace rtti::mmethod

#endif
