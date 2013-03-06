#ifndef RTTI_SHARED_TAGS_HPP
#define RTTI_SHARED_TAGS_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/has_dereference.hpp>

#include "rtti/hash/path.hpp"

// tag templates -> mark dispatch-guilty types (virtual) and the others
namespace rtti { namespace tags {

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
  : hash::make_path<T> {};
};

// compute dispatch arity
template<typename... Types>
struct virtual_size
: boost::mpl::count_if<
  boost::mpl::vector<Types...>
, is_virtual
> {};

} // namespace tags

} // namespace rtti

#endif
