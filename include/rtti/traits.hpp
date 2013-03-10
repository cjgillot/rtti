#ifndef RTTI_TRAITS_HPP
#define RTTI_TRAITS_HPP

#include <type_traits>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

namespace rtti {

namespace traits_detail {

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

template<typename T, typename U>
struct copy_ref<T&&,U> {
  typedef U&& type;
};

template<typename Out>
struct unsafe_casting {
  struct nonvirtual {
    template<typename In>
    static Out eval(In&& in)
    { return static_cast<Out>( std::forward<In>(in) ); }
  };
  struct dynamic {
    template<typename In>
    static Out eval(In&& in)
    { return dynamic_cast<Out>( std::forward<In>(in) ); }
  };

  template<typename In>
  static Out eval(In&& in) {
    constexpr bool dyn = boost::is_virtual_base_of<Out, In>::value;
    using impl = typename std::conditional<dyn, dynamic, nonvirtual>::type;
    return impl::eval(in);
  }
};

template<typename T>
struct class_traits {
  typedef T&  reference_type;
  typedef typename std::remove_cv<T>::type class_type;
  typedef class_type raw_type;

  static reference_type get(reference_type v) { return v; }
  static bool valid(reference_type v) { return true; }

  template<typename U>
  static U& cast(reference_type v)
  { return unsafe_casting<U>::eval(v); }

  template<typename U>
  struct rebind {
    typedef typename copy_cv<T,U>::type other;
  };
};

template<typename T>
struct fundamental_traits {
  typedef typename std::remove_cv<T>::type raw_type;
  typedef void class_type;

  template<typename U>
  static U cast(typename boost::call_traits<T>::param_type v)
  { return v; }

  template<typename U>
  struct rebind {
    typedef typename copy_cv<T,U>::type other;
  };
};

template<typename T>
struct smart_ptr_traits {
private:
  static T const& instance;

public:
  typedef decltype(*instance) reference_type;
  typedef typename std::remove_cv<
          typename std::remove_reference<
          reference_type
  >::type>::type class_type;
  typedef class_type raw_type;

  static reference_type get(T const& v) { return *v; }
  static bool valid(T const& v) { return bool(v); }
};

} // namespace

template<typename T>
struct pointer_traits
: std::conditional<
    std::is_class<T>::value
  , traits_detail::class_traits<T>
  , traits_detail::fundamental_traits<T>
>::type
{};

template<typename T>
struct pointer_traits<T*>
: traits_detail::smart_ptr_traits<T*> {
  template<typename PU>
  static PU cast(T* p) {
    typedef typename std::remove_pointer<PU>::type U;
    typedef rtti::traits_detail::unsafe_casting<U*> caster;
    return caster::eval( p );
  }

  template<typename U>
  struct rebind {
    typedef U* other;
  };
};

template<typename T>
struct pointer_traits<T* const>
: traits_detail::smart_ptr_traits<T*> {
  template<typename U>
  struct rebind {
    typedef U* const other;
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

template<typename T>
struct pointer_traits<T&&>
: pointer_traits<T> {
  template<typename U>
  struct rebind {
    typedef typename pointer_traits<T>::template rebind<U>::other&& other;
  };
};

} // namespace rtti

#endif