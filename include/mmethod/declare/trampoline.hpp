//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_TRAMPOLINE_HPP
#define RTTI_MMETHOD_TRAMPOLINE_HPP

#include "mmethod/config.hpp"

#include "mmethod/traits/pointer_traits.hpp"

#include "mmethod/detail/iterate.hpp"

#include <boost/call_traits.hpp>
#include <boost/mpl/size.hpp>

namespace rtti {
namespace mmethod {
namespace detail {
namespace trampoline_detail {

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
, typename compute_pointer_traits<In>::type
, other_caster<In>
>::type {};

template<std::size_t Arity>
struct trampoline_base;

} // namespace trampoline_detail
} // namespace detail
} // namespace mmethod
} // namespace rtti

#define BOOST_PP_FILENAME_1 "mmethod/declare/trampoline_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, MMETHOD_MAX_ITERATION)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS

namespace rtti {
namespace mmethod {
namespace detail {

template<typename TAG, typename Ret, typename Types, typename Tags>
struct make_trampoline
: trampoline_detail::trampoline_base<
    boost::mpl::size<Types>::value
>::template callback<
    TAG, Ret, Types, Tags
> {};

} // namespace detail
} // namespace mmethod
} // namespace rtti

#endif
