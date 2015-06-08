//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/config.hpp"

#include <boost/mpl/at.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

#include <boost/call_traits.hpp>

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
#include <utility>
#else
#include <boost/move/utility_core.hpp>
#endif
