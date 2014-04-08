//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/detail/prefix.hpp"

namespace boost {
namespace mmethod {
namespace call_detail {

template<>
struct make_declare_call_base<
  BOOST_PP_ITERATION()
> {

template<typename Tag, typename Ret, typename Args>
struct apply {
protected: // traits_type
  typedef detail::make_declare_traits<Ret, Args> traits_type;

private:
  typedef typename traits_type::unwrapped_args unwrapped_args;
  typedef typename traits_type::type_tags      type_tags;

protected: // trampoline_type
  typedef detail::make_trampoline<Tag, Ret, unwrapped_args, type_tags> trampoline_type;
  typedef typename trampoline_type::sig_t func_t;

private:   // dispatch_type
  typedef dispatch<Tag,Ret> dispatch_type;
  dispatch_type m_dispatch;

protected:
  template<typename K, typename F>
  inline void insert(F const& f)
  { m_dispatch.template insert<K>(f); }

  inline void generate()
  { m_dispatch.generate(); }

protected:

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), BOOST_MMETHOD_TRAMPOLINE_FUNC_ARG, BOOST_PP_EMPTY)

  inline func_t fetch(
    BOOST_MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    typedef boost::fusion::tuple<
      BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES(unwrapped_args)
    > tuple_type;
    invoker_t inv = m_dispatch.fetch( tuple_type(BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS) );
    return reinterpret_cast<func_t>(inv);
  }
  inline Ret operator()(
    BOOST_MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    func_t f = this->fetch(BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS);
    return (Ret) (*f)(BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

#undef BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS

}; // apply

}; // make_declare_call_base

}}} // namespace boost::mmethod::call_detail

#include "boost/mmethod/detail/suffix.hpp"
