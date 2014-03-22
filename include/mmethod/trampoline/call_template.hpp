//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/trampoline/prefix.hpp"

#define MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_ARG, BOOST_PP_EMPTY)

protected:
  inline func_t fetch(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    typedef boost::fusion::tuple<
      MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES(unwrapped_args)
    > tuple_type;
    invoker_t inv = m_dispatch.fetch( tuple_type(MMETHOD_TRAMPOLINE_FUNC_ARGS) );
    return reinterpret_cast<func_t>(inv);
  }
  inline Ret operator()(
    MMETHOD_TRAMPOLINE_FUNC_PARMS(unwrapped_args)
  ) const
  {
    func_t f = this->fetch(MMETHOD_TRAMPOLINE_FUNC_ARGS);
    return (Ret) (*f)(MMETHOD_TRAMPOLINE_FUNC_ARGS);
  }

#undef MMETHOD_TRAMPOLINE_FUNC_ARGS

#include "mmethod/trampoline/suffix.hpp"
