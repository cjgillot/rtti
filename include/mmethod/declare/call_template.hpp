//          Copyright Camille Gillot 2012 - 2016.
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
struct apply
{
private:
  typedef make_declare_call_common<Tag, Policy, Ret, Args> common_type;
  common_type m_common;

protected:
  typedef typename common_type::traits_type     traits_type;
  typedef typename common_type::unwrapped_args  unwrapped_args;
  typedef typename common_type::trampoline_type trampoline_type;
  typedef typename common_type::func_t          func_t;

protected:
  template<typename K>
  void insert(func_t const& f)
  { m_common.template insert<K>(f); }

  void generate() const
  { m_common.generate(); }

private:
  MMETHOD_TRAMPOLINE_DECLARE_TYPES(unwrapped_args)

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
    typedef boost::tuple<
      MMETHOD_TRAMPOLINE_CALL_PARM_TYPES(unwrapped_args)
    > tuple_type;
    invoker_t inv = m_common.fetch(
      tuple_type(MMETHOD_TRAMPOLINE_CALL_ARGS)
    );
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
    MMETHOD_TRAMPOLINE_CALL_PARMS(unwrapped_args)
  ) const
  {
    this->generate();
    return this->fast_fetch(MMETHOD_TRAMPOLINE_CALL_ARGS);
  }
  inline Ret call(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    this->generate();
    func_t f = this->fast_fetch(MMETHOD_TRAMPOLINE_CALL_ARGS);
    return (Ret) (*f)(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

  // cosmetic
  inline Ret operator()(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    this->generate();
    func_t f = this->fast_fetch(MMETHOD_TRAMPOLINE_CALL_ARGS);
    return (Ret) (*f)(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

  // super
  template<typename SuperTypes>
  BOOST_FORCEINLINE func_t fast_super_fetch() const
  {
    invoker_t inv = m_common.template super<SuperTypes>();
    return reinterpret_cast<func_t>(inv);
  }
  template<typename SuperTypes>
  BOOST_FORCEINLINE Ret fast_super(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    func_t f = this->template fast_super_fetch<SuperTypes>();
    return (Ret) (*f)(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

  // safe path : generate() first
  template<typename SuperTypes>
  inline func_t super_fetch() const
  {
    this->generate();
    return this->template fast_super_fetch<SuperTypes>();
  }
  template<typename SuperTypes>
  inline Ret super(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    this->generate();
    func_t f = this->template fast_super_fetch<SuperTypes>();
    return (Ret) (*f)(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

#undef MMETHOD_TRAMPOLINE_FUNC_ARGS
#undef MMETHOD_TRAMPOLINE_CALL_ARGS

}; // apply

}; // make_declare_call_base

} // namespace call_detail
} // namespace detail
} // namespace mmethod
} // namespace rtti

#include "mmethod/detail/suffix.hpp"
