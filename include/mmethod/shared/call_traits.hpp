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
