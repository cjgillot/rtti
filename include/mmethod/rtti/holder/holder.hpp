//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"
#include "mmethod/rtti/holder/node.hpp"

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_volatile.hpp>

namespace rtti {
namespace detail {
namespace holder_ {

//! \brief Structure holding class-wide rtti information
//! \note Arguments must be const-qualified to avoid unnecessary instanciations
template<class T>
struct holder {
private:
  BOOST_STATIC_ASSERT_MSG(
     boost::is_class<T>::value
  && boost::is_const<T>::value
  && boost::is_volatile<T>::value
  , "rtti::detail::holder_::holder<> must not be accessed directly"
  );

  typedef rtti_getter::traits<T> traits;

  BOOST_STATIC_CONSTANT(std::size_t, Arity = traits::parents_size);

  struct initializer_t {
    struct register_one;
    initializer_t();
    void touch() const {}
  };
  static initializer_t initializer;

  static rtti_node_var<Arity> node;

public:
  static inline
  const rtti_node*
  MMETHOD_ATTRIBUTE_PURE
  get_node() BOOST_NOEXCEPT_OR_NOTHROW
  { initializer.touch(); return reinterpret_cast<rtti_node*>(&node); }
};

//! \brief Grant access to the holder
template<typename T>
struct get_holder {
  typedef holder<T const volatile> type;
};

//! \brief Placeholder struct used for hierarchy base
template<>
struct get_holder<const volatile void> {
  struct type {
    static BOOST_CONSTEXPR rtti_node* get_node() { return NULL; }
  };
};

template<class T>
struct holder<T>::initializer_t::register_one {
  mutable std::size_t k;

  template<typename P>
  void operator()(P* /*unused*/) const {
    typedef typename get_holder<P>::type U;
    holder::node.__base[k] = U::get_node();
    ++k;
  }
};

template<class T>
holder<T>::initializer_t::initializer_t() {
  holder::node.__arity = Arity;

  register_one reg = { 0 };
  traits::parents_foreach(reg);
}

template<class T>
typename holder<T>::initializer_t
  holder<T>::initializer;

template<class T>
rtti_node_var<holder<T>::Arity>
  holder<T>::node;

} // namespace holder_

using holder_::get_holder;

} // namespace detail
} // namespace rtti

#endif
