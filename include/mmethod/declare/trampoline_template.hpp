//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

template<>
struct trampoline_base<
  BOOST_PP_ITERATION()
> {

template<typename TAG, typename Ret, typename Types, typename Tags>
struct callback {
  typedef Ret return_type;

#include "mmethod/detail/prefix.hpp"

#define MMETHOD_TRAMPOLINE_FUNC_CAST_ARG(J,I,D)      \
    trampoline_detail::caster<                  \
      MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Tags)    \
    , MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types)   \
    >::template cast<                           \
      MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types2)  \
    >( MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,D) )

#define MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_CAST_ARG, BOOST_PP_EMPTY)

  typedef Ret (*sig_t)(MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES(Types));

  template<typename Over, typename Ret2, typename Types2>
  struct apply {
    static Over over;
    static Ret call(
      MMETHOD_TRAMPOLINE_FUNC_PARMS(Types)
    ) MMETHOD_ATTRIBUTE_ALIGNED
    {
      return over.call(MMETHOD_TRAMPOLINE_FUNC_ARGS);
    }
  };

#undef MMETHOD_TRAMPOLINE_FUNC_ARGS
#undef MMETHOD_TRAMPOLINE_FUNC_CAST_ARG

#include "mmethod/detail/suffix.hpp"

}; // struct callback

}; // struct trampoline_base

template<typename TAG, typename Ret, typename Types, typename Tags>
template<typename Over, typename Ret2, typename Types2>
Over
  trampoline_base<BOOST_PP_ITERATION()>
  ::template callback<TAG, Ret, Types, Tags>
  ::apply<Over, Ret2, Types2>
  ::over;
