//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_CALL_TRAITS_HPP
#define MMETHOD_CALL_TRAITS_HPP

#include "mmethod/config.hpp"
#include <boost/call_traits.hpp>

namespace rtti {

template<typename T>
struct call_traits
: boost::call_traits<T>
{
private:
  typedef boost::call_traits<T> base;

public:
  using typename base::value_type;
  using typename base::reference;
  using typename base::const_reference;
  using typename base::param_type;
};

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

template<typename T>
struct call_traits<T&&> {
  typedef T&& value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T&& param_type;  // hh removed const
};

#endif

} // namespace rtti

#endif
