#ifndef RTTI_DMETHOD_DECLARE_HELPER_HPP
#define RTTI_DMETHOD_DECLARE_HELPER_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/shared/tags.hpp"

#include "mmethod/dynamic/common.hpp"
#include "mmethod/dispatch/dispatch.hpp"
#include "mmethod/trampoline/trampoline.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/function_types/components.hpp>

#include "mmethod/shared/call_traits.hpp"
#include <boost/fusion/tuple.hpp>

namespace rtti { namespace dmethod { namespace detail {
  
template<typename Tag2, typename Over2, typename Ret2, typename Args2>
struct make_implement_helper;

template<typename Tag, typename Ret, typename Args>
struct make_declare_helper {
protected:
  struct traits;

private:
  typedef detail::dispatch<Tag,Ret> dispatch_type;

  // grant access
  friend dispatch_type;
  friend class register_base<Tag>;
  template<typename Tag2, typename Over2, typename Ret2, typename Args2>
  friend struct make_implement_helper;

  dispatch_type m_dispatch;

protected:
  typedef make_declare_helper decl_maker;

//   template<typename... Args2>
//   inline Ret operator()(Args2&& ...args) const
//   { return (Ret) m_dispatch.call( boost::fusion::make_tuple( std::forward<Args2>(args)... ) ); }
#include "mmethod/trampoline/call.hpp"

protected:
  template<typename K, typename F>
  inline void insert(F&& f)
  { m_dispatch.template insert<K>( std::forward<F>(f) ); }

  inline void generate()
  { m_dispatch.generate(); }
};

template<typename Tag, typename Sig>
struct make_declare {
private:
  typedef boost::function_types::components<Sig> components;
  typedef typename boost::mpl::front<components>::type result;
  typedef typename boost::mpl::pop_front<components>::type args;
  
public:
  typedef make_declare_helper<Tag, result, args> type;
};

}}} // namespace rtti::mmethod::detail

#endif
