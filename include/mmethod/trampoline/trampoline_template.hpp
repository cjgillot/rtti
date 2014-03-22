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

#define MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,D) \
    typename boost::mpl::at_c< D, I >::type

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,D) \
    typename boost::call_traits< MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types) >::param_type

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE, BOOST_PP_EMPTY)

#define MMETHOD_TRAMPOLINE_FUNC_PARM(J,I,D) \
    MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,D) BOOST_JOIN(a,I)

#define MMETHOD_TRAMPOLINE_FUNC_PARMS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM, BOOST_PP_EMPTY)

#define MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,D)      \
    rtti::detail::caster<                       \
      MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Tags)    \
    , MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types)   \
    >::template cast<                           \
      MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types2)  \
    >( BOOST_JOIN(a,I) )

#define MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_ARG, BOOST_PP_EMPTY)

  typedef Ret (*sig_t)(MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES);

  template<typename Over, typename Ret2, typename Types2>
  struct apply {
    static Over over;
    static Ret call(MMETHOD_TRAMPOLINE_FUNC_PARMS) MMETHOD_ATTRIBUTE_ALIGNED
    { return over.call(MMETHOD_TRAMPOLINE_FUNC_ARGS); }
  };

#undef MMETHOD_TRAMPOLINE_FUNC_TYPE
#undef MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE
#undef MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES
#undef MMETHOD_TRAMPOLINE_FUNC_ARG
#undef MMETHOD_TRAMPOLINE_FUNC_ARGS
}; // struct callback

}; // struct trampoline_base

template<typename TAG, typename Ret, typename Types, typename Tags>
template<typename Over, typename Ret2, typename Types2>
Over
  trampoline_base<BOOST_PP_ITERATION()>
  ::template callback<TAG, Ret, Types, Tags>
  ::apply<Over, Ret2, Types2>
  ::over;
