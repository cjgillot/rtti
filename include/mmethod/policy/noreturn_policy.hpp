//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_NORETURN_HANDLER_HPP
#define MMETHOD_AMBIGUITY_NORETURN_HANDLER_HPP

#include <cstdlib>
#include <boost/function_types/result_type.hpp>

namespace rtti {
namespace mmethod {
namespace ambiguity {

template<typename Policy, typename Ret = void>
struct noreturn_policy {

  template<typename Sig>
  struct rebind {
    typedef typename boost::function_types::result_type<Sig>::type R2;
    typedef noreturn_policy<Policy, R2> other;
  };

#define MMETHOD_NOOP(z,I,D)                                     \
  template<BOOST_PP_ENUM_PARAMS_Z(z, I, class T)>               \
  static Ret bad_dispatch(BOOST_PP_ENUM_PARAMS_Z(z, I, T)) {    \
    Policy::bad_dispatch();                                     \
    std::abort();                                               \
  }
  BOOST_PP_REPEAT_FROM_TO(1, MMETHOD_MAX_ITERATION, MMETHOD_NOOP, ~)
#undef MMETHOD_NOOP

};

}}} // namespace rtti::mmethod::ambiguity

#endif
