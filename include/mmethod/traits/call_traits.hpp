//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_CALL_TRAITS_HPP
#define BOOST_MMETHOD_CALL_TRAITS_HPP

#include "boost/mmethod/config.hpp"
#include <boost/call_traits.hpp>

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

namespace boost {
  template<typename T>
  struct call_traits<T&&> {
    typedef T&& value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T&& param_type;  // hh removed const
  };
}

#endif

#endif
