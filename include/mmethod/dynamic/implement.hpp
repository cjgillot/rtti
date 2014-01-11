#ifndef RTTI_DMETHOD_IMPLEMENT_HPP
#define RTTI_DMETHOD_IMPLEMENT_HPP

#include "mmethod/dynamic/regbase.hpp"

#define MMETHOD_IMPLEMENT_CTOR_CODE()   \
  Tag().template insert<Args...>( &impl_maker::trampoline::call );

#define MMETHOD_NSPACE dmethod
#include "mmethod/templates/implement.hpp"
#undef MMETHOD_NSPACE

#undef MMETHOD_IMPLEMENT_CTOR_CODE

namespace rtti {
namespace dmethod {

template<typename Tag, typename Hashes, typename Sig>
struct dmethod_implementation
: detail::make_implement<Tag, typename Tag::template overload<Hashes>, Sig> {
  typedef typename dmethod_implementation::impl_maker impl_maker;

protected:
  dmethod_implementation() noexcept {}
};

#define __MMETHOD_HDECL_(tag, ret, sig)  \
  typename ::rtti::dmethod::detail::make_hashes<tag, ret sig>::hashes

#define IMPLEMENTATION_MMETHOD(tag, ret, sig)                   \
template<>                                                      \
struct tag::overload<__MMETHOD_HDECL_(tag, ret, sig)>           \
: public ::rtti::dmethod::dmethod_implementation<tag, __MMETHOD_HDECL_(tag, ret, sig), ret sig> {        \
  static ret call sig;                                          \
  overload() {}                                                 \
};                                                              \
ret tag::overload<__MMETHOD_HDECL_(tag, ret, sig)>              \
  ::call sig /* definition here */

#define IMPLEMENT_MMETHOD(name, ret, sig) \
  IMPLEMENTATION_MMETHOD(__MMETHOD_TAG(name), ret, sig)

}} // namespace rtti::mmethod

#endif
