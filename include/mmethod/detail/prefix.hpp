//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define MMETHOD_TRAMPOLINE_TYPE(J,I,TypeSeq) \
    typename boost::mpl::at_c< TypeSeq, I >::type

// {{{ Macros used in call.hpp

#define MMETHOD_TRAMPOLINE_CALL_ARG(J,I,TypeSeq) \
    BOOST_JOIN(a,I)

#define MMETHOD_TRAMPOLINE_CALL_PARM_TYPE(J,I,TypeSeq) \
    typename rtti::call_traits< MMETHOD_TRAMPOLINE_TYPE(J,I,TypeSeq) >::param_type

#define MMETHOD_TRAMPOLINE_CALL_PARM_TYPES(TypeSeq) \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_CALL_PARM_TYPE, TypeSeq)

#define MMETHOD_TRAMPOLINE_CALL_PARM(J,I,TypeSeq) \
    MMETHOD_TRAMPOLINE_CALL_PARM_TYPE(J,I,TypeSeq) BOOST_JOIN(a,I)

#define MMETHOD_TRAMPOLINE_CALL_PARMS(TypeSeq) \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_CALL_PARM, TypeSeq)

// }}}
// {{{ Macros used in trampoline.hpp

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES

#define MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,TypeSeq) \
    BOOST_JOIN(a,I)

#else

#define MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,TypeSeq)                \
    std::forward< MMETHOD_TRAMPOLINE_TYPE(J,I,TypeSeq) >(  \
      BOOST_JOIN(a,I)                                           \
    )

#endif

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,TypeSeq) \
    MMETHOD_TRAMPOLINE_TYPE(J,I,TypeSeq)

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES(TypeSeq) \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE, TypeSeq)

#define MMETHOD_TRAMPOLINE_FUNC_PARM(J,I,TypeSeq) \
    MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,TypeSeq) BOOST_JOIN(a,I)

#define MMETHOD_TRAMPOLINE_FUNC_PARMS(TypeSeq) \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM, TypeSeq)

// }}}
