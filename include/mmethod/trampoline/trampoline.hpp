//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_TRAMPOLINE_HPP
#define RTTI_MMETHOD_TRAMPOLINE_HPP

#include "rtti/detail/traits.hpp"
#include "mmethod/shared/tags.hpp"

#include <boost/fusion/tuple.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/fusion/include/size.hpp>

#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

namespace rtti {
namespace detail {

template<std::size_t Arity, typename TAG, typename Ret, typename Types>
struct trampoline_base;

#define BOOST_PP_FILENAME_1 "mmethod/trampoline/trampoline_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, 2)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS

template<typename TAG, typename Ret, typename Types>
struct trampoline
: trampoline_base<
    boost::fusion::tuple_size<Types>::value,
    TAG, Ret, Types
> {};

}} // namespace rtti::detail

#endif
