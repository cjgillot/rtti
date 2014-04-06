//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_CALL_HPP
#define RTTI_MMETHOD_DECLARE_CALL_HPP

#include "mmethod/config.hpp"

#include "mmethod/declare/traits.hpp"
#include "mmethod/declare/trampoline.hpp"

#include "mmethod/dispatch/forward.hpp"

#include "mmethod/detail/iterate.hpp"

#include <boost/mpl/size.hpp>
#include <boost/fusion/tuple.hpp>

namespace rtti {
namespace mmethod {
namespace detail {
namespace call_detail {

template<std::size_t Arity>
struct make_declare_call_base;

}}}} // namespace rtti::mmethod::detail::call_detail

#define BOOST_PP_FILENAME_1 "mmethod/declare/call_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, MMETHOD_MAX_ITERATION)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Tag, typename Ret, typename Args>
struct make_declare_call
: protected call_detail::make_declare_call_base<
    boost::mpl::size<Args>::value
>::template apply<
  Tag, Ret, Args
> {};

}}} // namespace rtti::mmethod::detail

#endif
