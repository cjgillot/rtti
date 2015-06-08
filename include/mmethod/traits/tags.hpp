//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_SHARED_TAGS_HPP
#define RTTI_SHARED_TAGS_HPP

#include "mmethod/config.hpp"
#include <boost/mpl/bool.hpp>
#include <boost/mpl/count_if.hpp>

// tag templates -> mark dispatch-guilty types (virtual) and the others
namespace rtti { namespace tags {

template<typename T> struct _v { typedef T type; };
template<typename T> struct _s { typedef T type; };

// get tag-free type
//@{
template<typename T>
struct unwrap_once
: unwrap_once<_s<T> >
{};
template<typename T>
struct unwrap_once<_v<T> > {
  typedef T type;
  typedef boost::mpl::true_ has_virtual;
};
template<typename T>
struct unwrap_once<_s<T> > {
  typedef T type;
  typedef boost::mpl::false_ has_virtual;
};

template<typename T>
struct unwrap_base {
private:
  typedef unwrap_once<T> unwrapped;

public:
  typedef typename unwrapped::type        type;
  typedef typename unwrapped::has_virtual has_virtual;
};

struct unwrap {
  template<typename T>
  struct apply {
    typedef typename unwrap_base<T>::type type;
  };
};
//@}

// mpl predicate
struct is_virtual {
  template<typename T>
  struct apply
  : unwrap_base<T>::has_virtual {};
};

// compute dispatch arity
template<typename Types>
struct virtual_size
: boost::mpl::count_if<Types, is_virtual> {};

}} // namespace rtti::tags

#endif
