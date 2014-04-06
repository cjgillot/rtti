//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,TypeSeq) \
    typename boost::mpl::at_c< TypeSeq, I >::type

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,TypeSeq) \
    typename boost::call_traits< BOOST_MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,TypeSeq) >::param_type

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES(TypeSeq) \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE, TypeSeq)

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM(J,I,TypeSeq) \
    BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,TypeSeq) BOOST_JOIN(a,I)

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_PARMS(TypeSeq) \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), BOOST_MMETHOD_TRAMPOLINE_FUNC_PARM, TypeSeq)

#define BOOST_MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,D) \
    BOOST_JOIN(a,I)
