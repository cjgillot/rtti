//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_CALL_HPP
#define RTTI_MMETHOD_DECLARE_CALL_HPP

#include "mmethod/declare/traits.hpp"
#include "mmethod/declare/trampoline.hpp"

#include "mmethod/dispatch/dispatch.hpp"

#include "mmethod/traits/call_traits.hpp"
#include <boost/fusion/tuple.hpp>

namespace rtti { namespace mmethod { namespace detail {

template<typename Tag, typename Ret, typename Args>
struct make_declare_call {
protected: // traits_type
  typedef make_declare_traits<Ret, Args> traits_type;

private:
  typedef typename traits_type::unwrapped_args unwrapped_args;
  typedef typename traits_type::type_tags      type_tags;

protected: // trampoline_type
  typedef make_trampoline<Tag, Ret, unwrapped_args, type_tags> trampoline_type;
  typedef typename trampoline_type::sig_t func_t;

private:   // dispatch_type
  typedef detail::dispatch<Tag,Ret> dispatch_type;
  dispatch_type m_dispatch;

protected:
#define BOOST_PP_FILENAME_1 "mmethod/declare/call_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, 2)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS

protected:
  template<typename K, typename F>
  inline void insert(F const& f)
  { m_dispatch.template insert<K>(f); }

  inline void generate()
  { m_dispatch.generate(); }
};

}}} // namespace rtti::mmethod::detail

#endif
