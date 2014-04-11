//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_AMBIGUITY_NORETURN_HANDLER_HPP
#define BOOST_MMETHOD_AMBIGUITY_NORETURN_HANDLER_HPP

#include "boost/mmethod/detail/iterate.hpp"

#include <cstdlib>
#include <boost/function_types/result_type.hpp>

namespace boost {
namespace mmethod {
namespace ambiguity {

template<typename Policy, typename Ret = void>
struct noreturn_policy;

template<typename Policy, typename Ret>
struct noreturn_policy {

  template<typename Sig>
  struct rebind {
    typedef typename boost::function_types::result_type<Sig>::type R2;
    typedef noreturn_policy<Policy, R2> other;
  };

#define BOOST_MMETHOD_NOOP(z,I,D)                               \
  template<BOOST_PP_ENUM_PARAMS_Z(z, I, class T)>               \
  static Ret bad_dispatch(BOOST_PP_ENUM_PARAMS_Z(z, I, T)) {    \
    Policy::bad_dispatch();                                     \
    return *(Ret*)NULL;                                         \
  }
  BOOST_PP_REPEAT_FROM_TO(1, BOOST_MMETHOD_MAX_ITERATION, BOOST_MMETHOD_NOOP, ~)
#undef BOOST_MMETHOD_NOOP

};

template<typename Policy>
struct noreturn_policy<Policy, void> {

  template<typename Sig>
  struct rebind {
    typedef typename boost::function_types::result_type<Sig>::type R2;
    typedef noreturn_policy<Policy, R2> other;
  };

#define BOOST_MMETHOD_NOOP(z,I,D)                                     \
  template<BOOST_PP_ENUM_PARAMS_Z(z, I, class T)>               \
  static void bad_dispatch(BOOST_PP_ENUM_PARAMS_Z(z, I, T)) {   \
    Policy::bad_dispatch();                                     \
  }
  BOOST_PP_REPEAT_FROM_TO(1, BOOST_MMETHOD_MAX_ITERATION, BOOST_MMETHOD_NOOP, ~)
#undef BOOST_MMETHOD_NOOP

};

}}} // namespace boost::mmethod::ambiguity

#endif
