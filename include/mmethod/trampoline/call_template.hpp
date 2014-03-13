//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#define MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,D) \
    typename rtti::detail::nth_deref< D, I >::type

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,D) \
    typename boost::call_traits< MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,unwrapped_args) >::param_type

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE, BOOST_PP_EMPTY)

#define MMETHOD_TRAMPOLINE_FUNC_PARM(J,I,D) \
    MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,D) BOOST_PP_CAT(a,I)

#define MMETHOD_TRAMPOLINE_FUNC_PARMS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM, BOOST_PP_EMPTY)

#define MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), a)

protected:
  inline Ret operator()(MMETHOD_TRAMPOLINE_FUNC_PARMS) const
  {
    typedef boost::fusion::tuple<MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES> tuple_type;
    return (Ret) m_dispatch.call( tuple_type(MMETHOD_TRAMPOLINE_FUNC_ARGS) );
  }

#undef MMETHOD_TRAMPOLINE_FUNC_TYPE
#undef MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE
#undef MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES
#undef MMETHOD_TRAMPOLINE_FUNC_PARM
#undef MMETHOD_TRAMPOLINE_FUNC_PARMS
#undef MMETHOD_TRAMPOLINE_FUNC_ARGS
