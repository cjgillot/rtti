#ifndef RTTI_MMETHOD_BASIC_HPP
#define RTTI_MMETHOD_BASIC_HPP

#include <stdexcept>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/has_dereference.hpp>

#include "rtti/mmethod/poles.hpp"

namespace rtti { namespace mmethod {

using detail::invoker_t;

struct bad_dispatch
: std::runtime_error {
  bad_dispatch();
  virtual ~bad_dispatch() noexcept;
};

// type traits utilities
namespace type_traits {

template<typename T>
struct remove_all
{ typedef T type; };
template<typename T>
struct remove_all<T*>
: remove_all<T> {};
template<typename T>
struct remove_all<T&>
: remove_all<T> {};
template<typename T>
struct remove_all<T&&>
: remove_all<T> {};
template<typename T>
struct remove_all<const T>
: remove_all<T> {};
template<typename T>
struct remove_all<volatile T>
: remove_all<T> {};
template<typename T>
struct remove_all<const volatile T>
: remove_all<T> {};

}

namespace detail {

template<typename T>
struct mmethod_hpath_make;

}

// tag templates -> mark dispatch-guilty types (virtual) and the others
namespace tags {

template<typename T> struct virtual_ { typedef T type; };
template<typename T> struct  static_ { typedef T type; };

// get tag-free type
//@{
  // base cases
template<typename T>
struct unwrap {
  typedef T type;
  typedef boost::mpl::false_ has_virtual;
};
template<typename T>
struct unwrap<virtual_<T>> {
  typedef T type;
  typedef boost::mpl::true_ has_virtual;
};
template<typename T>
struct unwrap<static_<T>> {
  typedef T type;
  typedef boost::mpl::false_ has_virtual;
};

  // pointer/reference cases
template<typename T>
struct unwrap<T*> {
  typedef typename unwrap<T>::type* type;
  typedef typename unwrap<T>::has_virtual has_virtual;
};
template<typename T>
struct unwrap<T&> {
  typedef typename unwrap<T>::type& type;
  typedef typename unwrap<T>::has_virtual has_virtual;
};
template<typename T>
struct unwrap<T&&> {
  typedef typename unwrap<T>::type&& type;
  typedef typename unwrap<T>::has_virtual has_virtual;
};
  // no array

  // cv-qualifiers
template<typename T>
struct unwrap<T const> {
  typedef typename unwrap<T>::type const type;
  typedef typename unwrap<T>::has_virtual has_virtual;
};
template<typename T>
struct unwrap<T volatile> {
  typedef typename unwrap<T>::type volatile type;
  typedef typename unwrap<T>::has_virtual has_virtual;
};
template<typename T>
struct unwrap<T const volatile> {
  typedef typename unwrap<T>::type const volatile type;
  typedef typename unwrap<T>::has_virtual has_virtual;
};

//@}

template<typename T>
struct rewrap
: boost::mpl::if_<
    typename unwrap<T>::has_virtual
  , virtual_<typename unwrap<T>::type>
  , static_ <typename unwrap<T>::type>
> {};

// mpl predicate
struct is_virtual {
  template<typename T>
  struct apply
  : unwrap<T>::has_virtual {};
};

struct make_hierarchy {
  template<typename T>
  struct apply
  : detail::mmethod_hpath_make<T> {};
};

// compute dispatch arity
template<typename... Types>
struct virtual_size
: boost::mpl::count_if<
  boost::mpl::vector<Types...>
, is_virtual
> {};

} // namespace tags

}} // rtti::mmethod

#include "rtti/mmethod/trampoline.hpp"

#endif
