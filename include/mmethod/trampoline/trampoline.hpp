//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_TRAMPOLINE_HPP
#define RTTI_MMETHOD_TRAMPOLINE_HPP

#include "mmethod/traits/pointer_traits.hpp"
#include "mmethod/traits/call_traits.hpp"

#include <boost/mpl/at.hpp>

#include <boost/fusion/tuple.hpp>
#include <boost/fusion/include/size.hpp>

#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

namespace rtti {
namespace detail {

template<typename In>
struct other_caster {
  // rely on implicit conversion
  template<typename Out>
  static Out cast(typename boost::call_traits<Out>::param_type x) {
    return x;
  }
};

template<typename IsVirtual, typename In>
struct caster
: boost::mpl::if_<
  IsVirtual
, pointer_traits<In>
, other_caster<In>
>::type {};

template<std::size_t Arity>
struct trampoline_base;

#define BOOST_PP_FILENAME_1 "mmethod/trampoline/trampoline_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, 2)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS

template<typename TAG, typename Ret, typename Types, typename Tags>
struct trampoline
: trampoline_base<
    boost::fusion::tuple_size<Types>::value
>::template callback<
    TAG, Ret, Types, Tags
> {};

}} // namespace rtti::detail

#endif
