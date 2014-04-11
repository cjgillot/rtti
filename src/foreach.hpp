//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FOREACH_HPP
#define FOREACH_HPP

#include "mmethod/config.hpp"

#ifdef BOOST_NO_CXX11_RANGE_BASED_FOR

#  include <boost/foreach.hpp>
#  define foreach BOOST_FOREACH

#else

#  define foreach(a,b) for(a : b)

#endif

#endif
