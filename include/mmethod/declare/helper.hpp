//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_HELPER_HPP
#define RTTI_MMETHOD_DECLARE_HELPER_HPP

#include "mmethod/config.hpp"
#include "mmethod/declare/call.hpp"
#include "mmethod/declare/traits.hpp"

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

// detail/access.hpp
struct access;

template<typename Tag, typename Policy, typename Ret, typename Args>
struct make_declare_helper
: protected make_declare_call<Tag, Policy, Ret, Args>
{
private:
  typedef make_declare_call<Tag, Policy, Ret, Args> call_helper;

private:
  friend struct rtti::mmethod::detail::access;

protected:
  typedef make_declare_helper decl_maker;
  typedef Ret result_type;
  typedef typename call_helper::func_t func_t;

  using call_helper::fast_fetch;
  using call_helper::fast_call;

  using call_helper::fetch;
  using call_helper::call;

  using call_helper::operator();

  using call_helper::insert;
  using call_helper::generate;
};

template<typename Tag, typename Policy, typename Sig>
struct make_declare {
private:
  typedef typename boost::function_types::result_type<Sig>::type result;
  typedef typename boost::function_types::parameter_types<Sig>::type args;

public:
  typedef make_declare_helper<Tag, Policy, result, args> type;
};

} // namespace detail
} // namespace mmethod
} // namespace rtti

#endif
