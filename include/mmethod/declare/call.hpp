//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_DECLARE_CALL_HPP
#define BOOST_MMETHOD_DECLARE_CALL_HPP

#include "boost/mmethod/config.hpp"

#include "boost/mmethod/declare/traits.hpp"
#include "boost/mmethod/declare/trampoline.hpp"

#include "boost/mmethod/dispatch/forward.hpp"

#include "boost/mmethod/detail/iterate.hpp"

#include <boost/mpl/size.hpp>
#include <boost/fusion/tuple.hpp>

namespace boost {
namespace mmethod {
namespace call_detail {

template<std::size_t Arity>
struct make_declare_call_base;

}}} // namespace boost::mmethod::call_detail

#define BOOST_PP_FILENAME_1 "boost/mmethod/declare/call_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_MMETHOD_MAX_ITERATION)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS

namespace boost {
namespace mmethod {
namespace detail {

template<typename Tag, typename Policy, typename Ret, typename Args>
struct make_declare_call
: protected call_detail::make_declare_call_base<
    boost::mpl::size<Args>::value
>::template apply<
  Tag, Policy, Ret, Args
> {};

}}} // namespace boost::mmethod::detail

#endif
