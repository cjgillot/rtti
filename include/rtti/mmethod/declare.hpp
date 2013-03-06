#ifndef RTTI_MMETHOD_DECLARE_HPP
#define RTTI_MMETHOD_DECLARE_HPP

#include "rtti/mmethod/common.hpp"
#include "rtti/mmethod/dispatch.hpp"

#include "rtti/shared/basic.hpp"
#include "rtti/shared/tags.hpp"

namespace rtti {
namespace mmethod {

namespace detail {

template<typename Tag2, typename Over2, typename Ret2, typename... Args2>
struct make_implement_helper;

template<typename Tag, typename Ret, typename... Args>
struct make_declare_helper {
protected:
  enum {
    vsize = tags::virtual_size<Args...>::value
  };

  struct traits;

private:
  typedef dispatch<Tag, Ret, typename tags::rewrap<Args>::type...> dispatch_t;
  typedef detail::trampoline<Tag, Ret, typename tags::unwrap<Args>::type...> trampoline;
  friend dispatch_t;
  template<typename Tag2, typename Over2, typename Ret2, typename... Args2>
  friend struct make_implement_helper;

  dispatch_t m_dispatch;

public:
  inline Ret operator()(typename tags::unwrap<Args>::type... args) const
  { return (Ret) m_dispatch.call(args...); }
};

template<typename Tag, typename Ret, typename... Args>
make_declare_helper<Tag, Ret, Args...> make_declare_help(Ret(*f)(Args...));

template<typename Tag, typename Sig>
struct make_declare
: decltype( make_declare_help<Tag>( std::declval<Sig*>() ) ) {};

} // namespace detail

template<typename Tag, typename Sig>
struct mmethod_register
: mmethod_register_base<Tag>
, detail::make_declare<Tag, Sig> {
private:
  typedef detail::make_declare<Tag, Sig> decl_maker;

public:
  using decl_maker::operator();

  // for internal use
  using decl_maker::traits;
};

}} // namespace rtti::mmethod

#define __MMETHOD_TAG(name) BOOST_PP_CAT(__rtti_mmethod_tags__, name)

#define DECLARE_MMETHOD(name, ret, sig) \
struct __MMETHOD_TAG(name)              \
: rtti::mmethod::mmethod_register<      \
  __MMETHOD_TAG(name)                   \
, ret sig   > {                         \
  template<typename> struct overload;   \
}; \
static __MMETHOD_TAG(name) name /* ; */

#include "rtti/mmethod/declare.tpp"

#endif
