//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/detail/prefix.hpp"

namespace rtti {
namespace mmethod {
namespace detail {
namespace trampoline_detail {

template<>
struct trampoline_base<
  BOOST_PP_ITERATION()
> {

template<typename TAG, typename Ret, typename Types, typename Tags>
struct callback {
 private:
  MMETHOD_TRAMPOLINE_DECLARE_TYPES(Tags)
  MMETHOD_TRAMPOLINE_DECLARE_TYPES(Types)

#define MMETHOD_TRAMPOLINE_FUNC_CAST_ARG(J,I,D) \
    trampoline_detail::caster<                  \
      MMETHOD_TRAMPOLINE_TYPE(J,I,Tags)    \
    , MMETHOD_TRAMPOLINE_TYPE(J,I,Types)   \
    >::template cast<                      \
      MMETHOD_TRAMPOLINE_TYPE(J,I,Types2)  \
    >( MMETHOD_TRAMPOLINE_FUNC_ARG(J,I,Types) )

#define MMETHOD_TRAMPOLINE_FUNC_ARGS \
    BOOST_PP_ENUM(BOOST_PP_ITERATION(), MMETHOD_TRAMPOLINE_FUNC_CAST_ARG, ~)

public:
  typedef Ret return_type;
  typedef Ret (*sig_t)(MMETHOD_TRAMPOLINE_FUNC_PARM_TYPES(Types));

  template<typename Over, typename Ret2, typename Types2>
  struct apply {
  private:
    MMETHOD_TRAMPOLINE_DECLARE_TYPES(Types2)
    static Over over;

  public:
    static Ret call(
      MMETHOD_TRAMPOLINE_FUNC_PARMS(Types)
    ) MMETHOD_ATTRIBUTE_ALIGNED
    {
      return over.call(MMETHOD_TRAMPOLINE_FUNC_ARGS);
    }
  };

#undef MMETHOD_TRAMPOLINE_FUNC_ARGS
#undef MMETHOD_TRAMPOLINE_FUNC_CAST_ARG

}; // struct callback

}; // struct trampoline_base

template<typename TAG, typename Ret, typename Types, typename Tags>
template<typename Over, typename Ret2, typename Types2>
Over
  trampoline_base<BOOST_PP_ITERATION()>
  ::template callback<TAG, Ret, Types, Tags>
  ::apply<Over, Ret2, Types2>
  ::over;

} // namespace trampoline_detail
} // namespace detail
} // namespace mmethod
} // namespace rtti

#include "mmethod/detail/suffix.hpp"
