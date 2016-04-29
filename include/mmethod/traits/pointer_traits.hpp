//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_TRAITS_HPP
#define RTTI_TRAITS_HPP

#include "mmethod/config.hpp"

#include <boost/mpl/if.hpp>

#include <boost/call_traits.hpp>
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

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
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

template<typename T, typename Ref>
class reference_traits {
  typedef T& reference_type;

public:
  typedef typename boost::remove_cv<T>::type class_type;

  static reference_type get(Ref v) { return v; }
  static bool valid(Ref /*unused*/) { return true; }

  template<typename U>
  static U cast(Ref v)
  {
    typedef typename boost::add_pointer<U>::type Uptr;
    Uptr up = unsafe_casting<Uptr>::eval(&v);
    return static_cast<U>(*up);
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

struct empty_pointer_traits {};

} // namespace traits_detail

// base case : empty class
template<typename T>
struct pointer_traits
: traits_detail::empty_pointer_traits // mark triviality
{};

template<typename T>
struct pointer_traits<T*>
: traits_detail::raw_ptr_traits<T>
{};

template<typename T>
struct compute_pointer_traits
{
private:
  typedef pointer_traits<T> traits_T;
  enum {
    trivial_T = boost::is_convertible<
      traits_T, traits_detail::empty_pointer_traits
    >::value
  };

  typedef typename boost::remove_cv<
    typename boost::remove_reference<T>::type
  >::type no_cvref_t;
  typedef pointer_traits<no_cvref_t> traits_no_cvref;
  enum {
    trivial_no_cvref = boost::is_convertible<
      traits_no_cvref, traits_detail::empty_pointer_traits
    >::value
  };

  typedef typename boost::remove_reference<
    typename boost::call_traits<T>::reference
  >::type complete_no_ref_t;

public:
  typedef typename boost::mpl::if_c<
      !trivial_T
    , traits_T
    , typename boost::mpl::if_c<
        !trivial_no_cvref
      , traits_no_cvref
      , traits_detail::reference_traits<complete_no_ref_t, T>
      >::type
    >::type type;
};

} // namespace rtti

#endif
