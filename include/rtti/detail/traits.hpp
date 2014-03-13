#ifndef RTTI_TRAITS_HPP
#define RTTI_TRAITS_HPP

#include <boost/type_traits.hpp>
#include <boost/type_traits/has_dereference.hpp>
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

#ifdef BOOST_HAS_RVALUE_REFS
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

template<typename T, typename U>
struct copy_cv {
  typedef U type;
};

template<typename T, typename U>
struct copy_cv<T const,U> {
  typedef U const type;
};

template<typename T, typename U>
struct copy_cv<T volatile,U> {
  typedef U volatile type;
};

template<typename T, typename U>
struct copy_cv<T const volatile,U> {
  typedef U const volatile type;
};

template<typename T, typename U>
struct copy_ref {
  typedef U type;
};

template<typename T, typename U>
struct copy_ref<T&,U> {
  typedef U& type;
};

#ifdef BOOST_HAS_RVALUE_REFS
template<typename T, typename U>
struct copy_ref<T&&,U> {
  typedef U&& type;
};
#endif

template<typename Out>
struct unsafe_casting {
  struct nonvirtual {
    template<typename In>
    static Out eval(In& in)
    { return static_cast<Out>( in ); }
  };
  struct dynamic {
    template<typename In>
    static Out eval(In& in)
    { return dynamic_cast<Out>( in ); }
  };

  template<typename In>
  static Out eval(In& in) {
    typedef typename remove_all<Out>::type out_class;
    typedef typename remove_all<In >::type  in_class;

    typedef boost::is_virtual_base_of<in_class, out_class> dyn;
    typedef typename boost::mpl::if_<dyn, dynamic, nonvirtual>::type impl;
    return impl::template eval<In>(in);
  }

  template<typename In>
  static Out eval(In const& in) {
    typedef typename remove_all<Out>::type out_class;
    typedef typename remove_all<In >::type  in_class;

    typedef boost::is_virtual_base_of<in_class, out_class> dyn;
    typedef typename boost::mpl::if_<dyn, dynamic, nonvirtual>::type impl;
    return impl::template eval<In const>(in);
  }
};

template<typename T>
struct class_traits {
  typedef T&  reference_type;
  typedef typename boost::remove_cv<T>::type class_type;
  typedef class_type raw_type;

  static reference_type get(reference_type v) { return v; }
  static bool valid(reference_type) { return true; }

  template<typename U>
  static U cast(reference_type v)
  { return unsafe_casting<U>::eval(v); }

  template<typename U>
  struct rebind {
    typedef typename copy_cv<T,U>::type other;
  };
};

template<typename T>
struct fundamental_traits {
  typedef typename boost::remove_cv<T>::type raw_type;
  typedef void class_type;

  template<typename U, typename V>
  static U cast(V& v)
  { return v; }

  template<typename U, typename V>
  static U cast(V const& v)
  { return v; }

  template<typename U>
  struct rebind {
    typedef typename copy_cv<T,U>::type other;
  };
};

template<typename T>
struct raw_ptr_traits;

template<typename T>
struct raw_ptr_traits<T*> {
public:
  typedef T& reference_type;
  typedef typename boost::remove_cv<
          typename boost::remove_reference<
          reference_type
  >::type>::type class_type;
  typedef class_type raw_type;

  static reference_type get(T* v) { return *v; }
  static bool valid(T* v) { return bool(v); }
};

template<typename T>
struct smart_ptr_traits {
public:
  typedef typename T::element_type& reference_type;
  typedef typename boost::remove_cv<
          typename boost::remove_reference<
          reference_type
  >::type>::type class_type;
  typedef class_type raw_type;

  static reference_type get(T const& v) { return *v; }
  static bool valid(T const& v) { return bool(v); }
};

template<typename T>
struct pointer_traits_basecase
: boost::mpl::if_<
    boost::is_class<T>
  , traits_detail::class_traits<T>
  , traits_detail::fundamental_traits<T>
>::type
{
  boost::true_type operator*() const;
};

template<typename CV, typename T>
struct wrap_cv_forward
: T {
  template<typename U>
  struct rebind {
  private:
    typedef typename T::template rebind<U>::other O;
  public:
    typedef typename copy_cv<CV, O>::type other;
  };
};

} // namespace

template<typename T>
struct pointer_traits
: traits_detail::pointer_traits_basecase<T>
{};

template<typename T>
struct pointer_traits<T const>
: boost::mpl::if_<
    boost::has_dereference< pointer_traits<T> >
  , traits_detail::pointer_traits_basecase<T const>
  , traits_detail::wrap_cv_forward<void const, pointer_traits<T> >
>::type
{};

template<typename T>
struct pointer_traits<T volatile>
: boost::mpl::if_<
    boost::has_dereference< pointer_traits<T> >
  , traits_detail::pointer_traits_basecase<T volatile>
  , traits_detail::wrap_cv_forward<void volatile, pointer_traits<T> >
>::type
{};

template<typename T>
struct pointer_traits<T const volatile>
: boost::mpl::if_<
    boost::has_dereference< pointer_traits<T const> >
  , typename boost::mpl::if_<
      boost::has_dereference< pointer_traits<T volatile> >
    , traits_detail::pointer_traits_basecase<T const volatile>
    , traits_detail::wrap_cv_forward<void const, pointer_traits<T volatile> >
  >::type
  , traits_detail::wrap_cv_forward<void volatile, pointer_traits<T const> >
>::type
{};

template<typename T>
struct pointer_traits<T*>
: traits_detail::smart_ptr_traits<T*> {
  template<typename PU>
  static PU cast(T* p) {
    typedef typename boost::remove_pointer<PU>::type U;
    typedef rtti::traits_detail::unsafe_casting<U*> caster;
    return caster::eval( p );
  }

  template<typename U>
  struct rebind {
    typedef U* other;
  };
};

template<typename T>
struct pointer_traits<T&>
: pointer_traits<T> {
  template<typename U>
  struct rebind {
    typedef typename pointer_traits<T>::template rebind<U>::other& other;
  };
};

#ifdef BOOST_HAS_RVALUE_REFS
template<typename T>
struct pointer_traits<T&&>
: pointer_traits<T> {
  template<typename U>
  struct rebind {
    typedef typename pointer_traits<T>::template rebind<U>::other&& other;
  };
};
#endif

} // namespace rtti

#endif
