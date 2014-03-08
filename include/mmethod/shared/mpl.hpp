#ifndef RTTI_MMETHOD_MPL_HPP
#define RTTI_MMETHOD_MPL_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/has_dereference.hpp>

namespace rtti {
namespace mpl {

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

}} // namespace rtti::mpl

#endif
