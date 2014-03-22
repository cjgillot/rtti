//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_HELPER_HPP
#define RTTI_MMETHOD_DECLARE_HELPER_HPP

#include "mmethod/declare/call.hpp"
#include "mmethod/declare/traits.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/function_types/components.hpp>

namespace rtti { namespace mmethod { namespace detail {

// detail/access.hpp
struct access;

template<typename Tag, typename Ret, typename Args>
struct make_declare_helper
: protected make_declare_call<Tag, Ret, Args>
{
private:
  typedef make_declare_call<Tag, Ret, Args> call_helper;

protected:
  typedef typename call_helper::traits_type traits_type;
  typedef typename call_helper::trampoline_type trampoline_type;

private:
  friend struct rtti::mmethod::detail::access;

protected:
  typedef make_declare_helper decl_maker;
  typedef Ret result_type;
  typedef typename call_helper::func_t func_t;

  using call_helper::fetch;
  using call_helper::insert;
  using call_helper::generate;
  using call_helper::operator();
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
