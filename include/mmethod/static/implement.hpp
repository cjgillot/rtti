#ifndef RTTI_MMETHOD_IMPLEMENT_HPP
#define RTTI_MMETHOD_IMPLEMENT_HPP

#include "mmethod/static/print.hpp"
#include "mmethod/static/regbase.hpp"

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Over>
struct force_instantiation {
  // function to force some instanciations
  template<std::size_t N>
  static void make() noexcept {
    typedef void(func_t)(boost::mpl::size_t<N-1>);
    func_t* volatile p; p = &instantiate_pole; (void)p;
  }

private:
  //! Instantiation guards function, these may not be called
  static void instantiate_pole(boost::mpl::size_t<0>) noexcept {
    void* volatile* aa = nullptr;
    *aa = &Over::template poles<0>::node;
  }
  template<std::size_t Index>
  static void instantiate_pole(boost::mpl::size_t<Index>) noexcept {
    void* volatile* aa = nullptr;
    *aa = &Over::template poles<Index>::node;
    instantiate_pole(boost::mpl::size_t<Index-1>{});
  }
};

}}} // namespace rtti::mmethod::detail

#define MMETHOD_IMPLEMENT_CTOR_CODE()   \
  force_instantiation<Over>::template make<sizeof...(Args)>();\
  typename Tag::traits t; (void)t; \
  RTTI_PRINT( typename impl_maker::decl );

#define MMETHOD_NSPACE mmethod
#include "mmethod/templates/implement.hpp"
#undef MMETHOD_NSPACE

#undef MMETHOD_IMPLEMENT_CTOR_CODE

namespace rtti {
namespace mmethod {

template<typename Tag, typename Hashes, typename Sig>
struct mmethod_implementation
: detail::make_implement<Tag, typename Tag::template overload<Hashes>, Sig> {
  typedef typename mmethod_implementation::impl_maker impl_maker;

protected:
  mmethod_implementation() noexcept {
  }
};

#define __MMETHOD_HDECL_(tag, ret, sig)  \
  typename ::rtti::mmethod::detail::make_hashes<tag, ret sig>::hashes

#define IMPLEMENTATION_MMETHOD(tag, ret, sig)                   \
template<>                                                      \
struct tag::overload<__MMETHOD_HDECL_(tag, ret, sig)>           \
: public ::rtti::mmethod::mmethod_implementation<tag, __MMETHOD_HDECL_(tag, ret, sig), ret sig> {        \
  template<size_t I> struct poles {                             \
    static ::rtti::rtti_node const* node;                       \
  };                                                            \
  static ret call sig;                                          \
  static const ::rtti::invoker_t address;                       \
  overload() {}                                                 \
};                                                              \
template<std::size_t I>                                         \
::rtti::rtti_node const* tag::overload<                         \
  __MMETHOD_HDECL_(tag, ret, sig)>::poles<I>::node              \
    = impl_maker::get_node<I>::value;                           \
::rtti::invoker_t const                                         \
  tag::overload<__MMETHOD_HDECL_(tag, ret, sig)>::address       \
    = (::rtti::invoker_t)&trampoline::call;            \
ret tag::overload<__MMETHOD_HDECL_(tag, ret, sig)>              \
  ::call sig /* definition here */

#define IMPLEMENT_MMETHOD(name, ret, sig) \
  IMPLEMENTATION_MMETHOD(__MMETHOD_TAG(name), ret, sig)

}} // namespace rtti::mmethod

#endif
