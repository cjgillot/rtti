//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_CALL_TRAITS_HPP
#define MMETHOD_CALL_TRAITS_HPP

#include <boost/call_traits.hpp>

#if __cplusplus >= 201103L

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
