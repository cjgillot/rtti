//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_TRAITS_HPP
#define RTTI_TRAITS_HPP

#include <boost/mpl/if.hpp>

#include <boost/type_traits.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

namespace rtti {
namespace traits_detail {

template<typename T>
struct remove_all
{ typedef T type; };
template<typename T>
struct remove_all<T*>
: remove_all<T> {};
template<typename T>
struct remove_all<T&>
: remove_all<T> {};

#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES
template<typename T>
struct remove_all<T&&>
: remove_all<T> {};
#endif

template<typename T>
struct remove_all<const T>
: remove_all<T> {};
template<typename T>
struct remove_all<volatile T>
: remove_all<T> {};
template<typename T>
struct remove_all<const volatile T>
: remove_all<T> {};

// works only on _pointers_
template<typename Out>
struct unsafe_casting {
  struct nonvirtual {
    template<typename In>
    static Out eval(In* in)
    { return static_cast<Out>( in ); }
  };
  struct dynamic {
    template<typename In>
    static Out eval(In* in)
    { return dynamic_cast<Out>( in ); }
  };

  template<typename In>
  static Out eval(In* in) {
    typedef typename remove_all<Out>::type out_class;
    typedef typename remove_all<In >::type  in_class;

    typedef boost::is_virtual_base_of<in_class, out_class> dyn;
    typedef typename boost::mpl::if_<dyn, dynamic, nonvirtual>::type impl;
    return impl::template eval<In>(in);
  }
};

template<typename T>
class reference_traits {
  typedef T& reference_type;

public:
  typedef typename boost::remove_cv<T>::type class_type;

  static reference_type get(reference_type v) { return v; }
  static bool valid(reference_type) { return true; }

  template<typename U>
  static U cast(reference_type v)
  {
    typedef typename boost::add_pointer<U>::type Uptr;
    return *unsafe_casting<Uptr>::eval(&v);
  }
};

template<typename T>
class raw_ptr_traits {

  typedef typename boost::remove_pointer<T>::type& reference_type;

public:
  typedef typename boost::remove_cv<
          typename boost::remove_reference<
          reference_type
  >::type>::type class_type;

  static reference_type get(T* v) { return *v; }
  static bool valid(T* v) { return bool(v); }

  template<typename U>
  static U cast(T* v)
  { return unsafe_casting<U>::eval(v); }
};

} // namespace traits_detail

// base case : const reference to T
template<typename T>
struct pointer_traits
: boost::mpl::if_<
  boost::is_pointer<T>
, traits_detail::raw_ptr_traits<typename boost::remove_pointer<T>::type>
, traits_detail::reference_traits<T const>
>::type
{};

template<typename T>
struct pointer_traits<T&>
: traits_detail::reference_traits<T> {};

// swallow [const&]
template<typename T>
struct pointer_traits<T const&>
: pointer_traits<T> {};

template<typename T>
struct pointer_traits<T volatile&>
: pointer_traits<T volatile> {};

template<typename T>
struct pointer_traits<T const volatile&>
: pointer_traits<T volatile> {};

} // namespace rtti

#endif
