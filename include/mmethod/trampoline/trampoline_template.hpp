//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

template<typename TAG, typename Ret, typename Types>
struct trampoline_base<
  BOOST_PP_ITERATION()
, TAG, Ret, Types
> {
  typedef Ret return_type;
  
  typedef typename boost::mpl::transform<Types, tags::unwrap>::type unwrapped_args;

#define MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,D) \
    typename boost::mpl::at_c< D, I >::type

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,D) \
    typename boost::call_traits< MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,unwrapped_args) >::param_type

#define MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE, BOOST_PP_EMPTY)

#define MMETHOD_TRAMPOLINE_FUNC_PARM(J,I,D) \
    MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE(J,I,D) BOOST_PP_CAT(a,I)

#define MMETHOD_TRAMPOLINE_FUNC_PARMS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_PARM, BOOST_PP_EMPTY)

#define MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,D) \
    rtti::pointer_traits< MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types) >::template cast< MMETHOD_TRAMPOLINE_FUNC_TYPE(J,I,Types2) >( BOOST_PP_CAT(a,I) )

#define MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_ARG, BOOST_PP_EMPTY)

  typedef Ret (*sig_t)(MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES);

  template<typename Over, typename Ret2, typename Types2>
  struct apply {
    static Over over;
    static Ret call(MMETHOD_TRAMPOLINE_FUNC_PARMS) MMETHOD_ATTRIBUTE_ALIGNED
    { return over.call(MMETHOD_TRAMPOLINE_FUNC_ARGS); }
  };

//   template<typename Ret2, typename... Types2, typename Func>
//   static std::function<sig_t> functor(Func&& f) {
//     return [f](typename tags::unwrap_base<Types>::arg_type... args) -> Ret
//     { return f( rtti::pointer_traits<Types>::template cast<Types2>(args)... ); };
//   }

#undef MMETHOD_TRAMPOLINE_FUNC_TYPE
#undef MMETHOD_TRAMPOLINE_FUNC_PARM_TYPE
#undef MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES
#undef MMETHOD_TRAMPOLINE_FUNC_ARG
#undef MMETHOD_TRAMPOLINE_FUNC_ARGS
};

template<typename TAG, typename Ret, typename Types>
template<typename Over, typename Ret2, typename Types2>
Over trampoline_base<BOOST_PP_ITERATION(), TAG, Ret, Types>::apply<Over, Ret2, Types2>::over;
