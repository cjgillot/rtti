#ifndef RTTI_MMETHOD_IMPLEMENT_HPP
#define RTTI_MMETHOD_IMPLEMENT_HPP

#include "rtti/mmethod/print.hpp"
#include "rtti/mmethod/regbase.hpp"

#include "rtti/shared/mpl.hpp"
#include "rtti/shared/mplpack.hpp"
#include "rtti/shared/trampoline.hpp"

#include <boost/mpl/bind.hpp>

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

template<typename Tag, typename Over, typename Ret, typename... Args>
struct make_implement_helper {
private:
  enum {
    vsize = Tag::vsize
  };

  typedef mpl::mplpack<Args...> args;

protected:
  typedef typename Tag::trampoline::template apply<Over, Ret, Args...> trampoline;

  template<std::size_t I>
  struct get_node {
  private:
    using arg_t = typename args::template get<I>::type;
    using klass_t = typename rtti::mpl::remove_all<arg_t>::type;
    using virtual_t = typename Tag::traits::type_tags::template get<I>::type;
    using dispatch_t = typename boost::mpl::if_<
      virtual_t
    , ::rtti::detail::get_holder<klass_t>
    , ::rtti::detail::invalid_node
    >::type::type;

  public:
    constexpr static rtti_node const* value = &dispatch_t::node;
  };

  typedef make_implement_helper impl_maker;

  // constructor to force some instanciations
  make_implement_helper() noexcept
  { force_instantiation<Over>::template make<sizeof...(Args)>(); }

#ifdef RTTI_MMETHOD_DO_PRINT
private:
  typedef typename args::template transform2<
    typename Tag::traits::type_tags
  , boost::mpl::if_<
      _2
    , boost::mpl::bind<tags::make_hierarchy, _1>
    , rtti::hash::path_node<0, rtti::hash::path_end>
    >
  >::type type_hiers;

public:
  typedef mmethod_implement<Tag, vsize, sizeof...(Args), type_hiers> decl;
#endif
};

template<typename Tag, typename Over, typename Ret, typename... Args>
make_implement_helper<Tag, Over, Ret, Args...> make_implement_help(Ret(*f)(Args...));

template<typename Tag, typename Over, typename Sig>
struct make_implement
: decltype( make_implement_help<Tag, Over>( std::declval<Sig*>() ) ) {};

template<typename Tag, typename Ret, typename... Args>
struct make_hashes_helper {
  typedef mpl::mplpack<Args...> args;
  typedef typename args::template transform2<
    typename Tag::traits::type_tags
  , boost::mpl::if_<
      _2
    , boost::mpl::bind<hash::detail::hash, _1>
    , boost::mpl::size_t<0>
    >
  >::type hashes_t;
  typedef typename hashes_t::as_pack_c::type hashes;
};

template<typename Tag, typename Ret, typename... Args>
make_hashes_helper<Tag, Ret, Args...> make_hashes_help(Ret(*f)(Args...));

template<typename Tag, typename Sig>
struct make_hashes
: decltype( make_hashes_help<Tag>( std::declval<Sig*>() ) ) {};

} // namespace detail

template<typename Tag, typename Hashes, typename Sig>
struct mmethod_implementation
: detail::make_implement<Tag, typename Tag::template overload<Hashes>, Sig> {
  typedef typename mmethod_implementation::impl_maker impl_maker;

protected:
  mmethod_implementation() noexcept {
    /// output implementation
    // this line prints mmethod_declare<>
    // must be before RTTI_PRINT( typename impl_maker::decl )
    typename Tag::traits t; (void)t;
    // print mmethod_implement<>
    RTTI_PRINT( typename impl_maker::decl );
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
