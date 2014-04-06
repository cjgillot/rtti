//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/detail/prefix.hpp"

namespace boost {
namespace mmethod {
namespace trampoline_detail {

template<>
struct trampoline_base<
  BOOST_PP_ITERATION()
> {

template<typename TAG, typename Ret, typename Types, typename Tags>
struct callback {
  typedef Ret return_type;

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_CAST_ARG(J,I,D)      \
    trampoline_detail::caster<                  \
      BOOST_MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Tags)    \
    , BOOST_MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types)   \
    >::template cast<                           \
      BOOST_MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types2)  \
    >( BOOST_MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,D) )

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), BOOST_MMETHOD_TRAMPOLINE_FUNC_CAST_ARG, BOOST_PP_EMPTY)

  typedef Ret (*sig_t)(BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES(Types));

  template<typename Over, typename Ret2, typename Types2>
  struct apply {
    static Over over;
    static Ret call(
      BOOST_MMETHOD_TRAMPOLINE_FUNC_PARMS(Types)
    ) BOOST_MMETHOD_ATTRIBUTE_ALIGNED
    {
      return over.call(BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS);
    }
  };

#undef BOOST_MMETHOD_TRAMPOLINE_FUNC_ARGS
#undef BOOST_MMETHOD_TRAMPOLINE_FUNC_CAST_ARG

}; // struct callback

}; // struct trampoline_base

template<typename TAG, typename Ret, typename Types, typename Tags>
template<typename Over, typename Ret2, typename Types2>
Over
  trampoline_base<BOOST_PP_ITERATION()>
  ::template callback<TAG, Ret, Types, Tags>
  ::apply<Over, Ret2, Types2>
  ::over;

}}} // namespace boost::mmethod::trampoline_detail

#include "boost/mmethod/detail/suffix.hpp"
