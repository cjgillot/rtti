//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FOREACH_HPP
#define FOREACH_HPP

#include "mmethod/config.hpp"

#include <boost/bind.hpp>
#include <boost/range/counting_range.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <boost/foreach.hpp>

#ifdef BOOST_NO_CXX11_RANGE_BASED_FOR
#  define foreach BOOST_FOREACH
#else
#  define foreach(a,b) for(a : b)
#endif

#define foreach_base(a,hierarchy)                                       \
    if(rtti_hierarchy BOOST_FOREACH_ID(hh) = NULL) {} else              \
    if(!(BOOST_FOREACH_ID(hh) = (hierarchy))) {} else                   \
    foreach(a, (boost::adaptors::transform(                             \
                boost::counting_range(0ul,                              \
                    rtti_get_base_arity(BOOST_FOREACH_ID(hh))),         \
                boost::bind(&rtti_get_base, BOOST_FOREACH_ID(hh), _1))))

#endif
