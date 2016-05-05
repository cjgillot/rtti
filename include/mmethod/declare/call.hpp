//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_CALL_HPP
#define RTTI_MMETHOD_DECLARE_CALL_HPP

#include "mmethod/config.hpp"

#include "mmethod/declare/traits.hpp"
#include "mmethod/declare/trampoline.hpp"

#include "mmethod/dispatch/forward.hpp"

#include "mmethod/detail/iterate.hpp"

#include <boost/mpl/size.hpp>
#include <boost/tuple/tuple.hpp>

namespace rtti {
namespace mmethod {
namespace detail {
namespace call_detail {

template<std::size_t Arity>
struct make_declare_call_base;

template<typename Tag,
         typename Policy,
         typename Ret,
         typename Args>
struct make_declare_call_common {
public:
  // traits_type
  typedef make_declare_traits<Ret, Policy, Args> traits_type;

  typedef typename traits_type::unwrapped_args unwrapped_args;
  typedef typename traits_type::type_tags      type_tags;

  // trampoline_type
  typedef make_trampoline<Tag, Ret, unwrapped_args, type_tags> trampoline_type;
  typedef typename trampoline_type::sig_t func_t;

  // dispatch_type
  typedef dispatch<Tag> dispatch_type;

  template<typename Tuple>
  invoker_t fetch(Tuple const& a) const
  { return m_dispatch.fetch(a); }

  template<typename K>
  void insert(func_t const& f)
  { m_dispatch.template insert<K>(f); }

  void generate() const
  { m_dispatch.template generate<unwrapped_args>(); }

private: // dispatch member variable
  dispatch_type m_dispatch;
};

} // namespace call_detail
} // namespace detail
} // namespace mmethod
} // namespace rtti

#define BOOST_PP_FILENAME_1 "mmethod/declare/call_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, MMETHOD_MAX_ITERATION)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Tag, typename Policy, typename Ret, typename Args>
struct make_declare_call
: protected call_detail::make_declare_call_base<
    boost::mpl::size<Args>::value
>::template apply<
  Tag, Policy, Ret, Args
> {};

} // namespace detail
} // namespace mmethod
} // namespace rtti

#endif
