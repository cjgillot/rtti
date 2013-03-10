#ifndef RTTI_SHARED_TAGS_HPP
#define RTTI_SHARED_TAGS_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/has_dereference.hpp>

#include <boost/call_traits.hpp>

#include "rtti/hash/path.hpp"

// tag templates -> mark dispatch-guilty types (virtual) and the others
namespace rtti { namespace tags {

template<typename T> struct virtual_ { typedef T type; };
template<typename T> struct  static_ { typedef T type; };

// get tag-free type
//@{
template<typename T>
struct unwrap_once {
  typedef T type;
  typedef boost::mpl::false_ has_virtual;
  constexpr static std::size_t hash = 0;
};
template<typename T>
struct unwrap_once<virtual_<T>> {
  typedef T type;
  typedef boost::mpl::true_  has_virtual;
  constexpr static std::size_t hash = rtti::hash::detail::hash::apply<type>::value;
};
template<typename T>
struct unwrap_once<static_<T>> {
  typedef T type;
  typedef boost::mpl::false_ has_virtual;
  constexpr static std::size_t hash = 0;
};

template<typename T>
struct unwrap {
private:
  typedef rtti::pointer_traits<T> traits;
  typedef typename traits::raw_type deref_t;

  typedef unwrap_once<deref_t> unwrapped;

public:
  typedef typename unwrapped::type raw_type;
  typedef typename traits::template rebind<raw_type>::other type;
  typedef typename boost::call_traits<type>::param_type arg_type;
  typedef typename unwrapped::has_virtual has_virtual;
  constexpr static std::size_t hash = unwrapped::hash;
};

template<typename T>
struct rewrap
: boost::mpl::if_<
    typename unwrap<T>::has_virtual
  , virtual_<typename unwrap<T>::type>
  , static_ <typename unwrap<T>::type>
> {};
//@}

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
