//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_HELPER_HPP
#define RTTI_MMETHOD_DECLARE_HELPER_HPP

#include "mmethod/dispatch/forward.hpp"
#include "mmethod/trampoline/trampoline.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/function_types/components.hpp>

// for trampoline/call.hpp
#include "mmethod/traits/call_traits.hpp"
#include "mmethod/traits/tags.hpp"
#include <boost/fusion/tuple.hpp>

namespace rtti { namespace mmethod { namespace detail {
  
template<typename Tag2, typename Over2, typename Ret2, typename Args2>
struct make_implement_helper;

template<typename Tag, typename Ret, typename Args>
struct make_declare_helper {
protected:
  struct traits;

private:
  friend struct detail::dispatch<Tag,Ret>;
  typedef detail::dispatch<Tag,Ret> dispatch_type;

  // grant access
  friend struct register_base<Tag>;
  template<typename Tag2, typename Over2, typename Ret2, typename Args2>
  friend struct make_implement_helper;

  dispatch_type m_dispatch;

protected:
  typedef make_declare_helper decl_maker;
  typedef Ret result_type;
  typedef typename traits::trampoline::sig_t func_t;

//   template<typename... Args2>
//   inline Ret operator()(Args2&& ...args) const
//   { return (Ret) m_dispatch.call( boost::fusion::make_tuple( std::forward<Args2>(args)... ) ); }
#include "mmethod/trampoline/call.hpp"

protected:
  template<typename K, typename F>
  inline void insert(F const& f)
  { m_dispatch.template insert<K>(f); }

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
