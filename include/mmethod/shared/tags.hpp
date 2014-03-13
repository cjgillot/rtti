//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_SHARED_TAGS_HPP
#define RTTI_SHARED_TAGS_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/has_dereference.hpp>

#include "mmethod/shared/call_traits.hpp"

// tag templates -> mark dispatch-guilty types (virtual) and the others
namespace rtti { namespace tags {

template<typename T> struct _v { typedef T type; };
template<typename T> struct _s { typedef T type; };

// get tag-free type
//@{
template<typename T>
struct unwrap_once {
  typedef T type;
  typedef boost::mpl::false_ has_virtual;
//   constexpr static std::size_t hash = 0;
};
template<typename T>
struct unwrap_once<_v<T> > {
  typedef T type;
  typedef boost::mpl::true_  has_virtual;
//   constexpr static std::size_t hash = rtti::hash::detail::hash::apply<type>::value;
};
template<typename T>
struct unwrap_once<_s<T> > {
  typedef T type;
  typedef boost::mpl::false_ has_virtual;
//   constexpr static std::size_t hash = 0;
};

template<typename T>
struct unwrap_base {
private:
  typedef rtti::pointer_traits<T> traits;
  typedef typename traits::raw_type deref_t;

  typedef unwrap_once<deref_t> unwrapped;

public:
  typedef typename unwrapped::type raw_type;
  typedef typename traits::template rebind<raw_type>::other type;
  typedef typename boost::call_traits<type>::param_type arg_type;
  typedef typename unwrapped::has_virtual has_virtual;
//   constexpr static std::size_t hash = unwrapped::hash;
};

struct unwrap {
  template<typename T>
  struct apply {
    typedef typename unwrap_base<T>::type type;
  };
};

// template<typename T>
// struct rewrap
// : boost::mpl::if_<
//     typename unwrap_base<T>::has_virtual
//   , _v<typename unwrap_base<T>::type>
//   , _s<typename unwrap_base<T>::type>
// > {};
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

} // namespace tags

} // namespace rtti

#endif
