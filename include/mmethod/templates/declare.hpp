#include "mmethod/shared/trampoline.hpp"
#include "mmethod/shared/basic.hpp"
#include "mmethod/shared/tags.hpp"

namespace rtti {
namespace MMETHOD_NSPACE {
namespace detail {

template<typename Tag2, typename Over2, typename Ret2, typename... Args2>
struct make_implement_helper;

template<typename Tag, typename Ret, typename... Args>
struct make_declare_helper {
protected:
  struct traits;

private:
  typedef detail::dispatch<Tag,Ret> dispatch_type;

  // grant access
  friend dispatch_type;
  friend class register_base<Tag>;
  template<typename Tag2, typename Over2, typename Ret2, typename... Args2>
  friend struct make_implement_helper;

  dispatch_type m_dispatch;

protected:
  template<typename... Args2>
  inline Ret operator()(Args2&& ...args) const
  { return (Ret) m_dispatch.call( std::forward<Args2>(args)... ); }

  template<typename... K, typename F>
  inline void insert(F&& f)
  { m_dispatch.template insert<K...>( std::forward<F>(f) ); }

  template<typename... K, typename F>
  inline void retract(F&& f)
  { m_dispatch.template retract<K...>(); }
};

template<typename Tag, typename Ret, typename... Args>
make_declare_helper<Tag, Ret, Args...> make_declare_help(Ret(*f)(Args...));

template<typename Tag, typename Sig>
using make_declare = decltype( make_declare_help<Tag>( std::declval<Sig*>() ) );

}}} // namespace rtti::mmethod::detail