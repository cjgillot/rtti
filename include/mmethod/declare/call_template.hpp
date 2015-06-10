//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/detail/prefix.hpp"

namespace rtti {
namespace mmethod {
namespace detail {
namespace call_detail {

template<>
struct make_declare_call_base<
  BOOST_PP_ITERATION()
> {

template<typename Tag, typename Policy, typename Ret, typename Args>
struct apply {
protected: // traits_type
  typedef make_declare_traits<Ret, Policy, Args> traits_type;

private:
  typedef typename traits_type::unwrapped_args unwrapped_args;
  typedef typename traits_type::type_tags      type_tags;

protected: // trampoline_type
  typedef make_trampoline<Tag, Ret, unwrapped_args, type_tags> trampoline_type;
  typedef typename trampoline_type::sig_t func_t;

private:   // dispatch_type
  typedef dispatch<Tag,Ret> dispatch_type;
  dispatch_type m_dispatch;

protected:
  template<typename K, typename F>
  void insert(F const& f)
  { m_dispatch.template insert<K>(f); }

  void generate() const
  { m_dispatch.template generate<unwrapped_args>(); }

protected:

#define MMETHOD_TRAMPOLINE_CALL_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_CALL_ARG, unwrapped_args)
#define MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_ARG, unwrapped_args)

  // fast path : no check for generate()
  BOOST_FORCEINLINE func_t fast_fetch(
    MMETHOD_TRAMPOLINE_CALL_PARMS(unwrapped_args)
  ) const
  {
    typedef boost::fusion::tuple<
      MMETHOD_TRAMPOLINE_CALL_PARM_TYPES(unwrapped_args)
    > tuple_type;
    invoker_t inv = m_dispatch.fetch( tuple_type(MMETHOD_TRAMPOLINE_CALL_ARGS) );
    return reinterpret_cast<func_t>(inv);
  }
  BOOST_FORCEINLINE Ret fast_call(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    func_t f = this->fast_fetch(MMETHOD_TRAMPOLINE_CALL_ARGS);
    return (Ret) (*f)(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

  // safe path : generate() first
  inline func_t fetch(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    this->generate();
    return this->fast_fetch( MMETHOD_TRAMPOLINE_FUNC_ARGS );
  }
  inline Ret call(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    this->generate();
    return (Ret) this->fast_call(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

  // cosmetic
  inline Ret operator()(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    return (Ret) this->call(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

#undef MMETHOD_TRAMPOLINE_FUNC_ARGS

}; // apply

}; // make_declare_call_base

}}}} // namespace rtti::mmethod::detail::call_detail

#include "mmethod/detail/suffix.hpp"
