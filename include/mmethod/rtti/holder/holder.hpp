//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include "mmethod/rttifwd.hpp"
#include "mmethod/rtti/holder/node.hpp"

namespace rtti {
namespace detail {
namespace holder_ {

template<typename> struct holder;

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

//! Arguments must be const-qualified to avoid unnecessary instanciations
template<class T>
struct holder {
private:
  BOOST_STATIC_ASSERT_MSG(
    boost::is_const<T>::value && boost::is_volatile<T>::value,
    "rtti::detail::holder_::holder<> must not be accessed directly"
  );

  BOOST_STATIC_CONSTANT(std::size_t, Arity = sizeof( rtti_parents_size_1p((T*)NULL) ) - 1);

  struct initializer_t {
    struct register_one;
    initializer_t();
    void touch() const {};
  };
  static initializer_t initializer;

public:
  static rtti_node_var<Arity> node;

  static inline
  const rtti_node*
  MMETHOD_ATTRIBUTE_PURE
  get_node() BOOST_NOEXCEPT_OR_NOTHROW
  { initializer.touch(); return reinterpret_cast<rtti_node*>(&node); }

  static inline
  rtti_type
  MMETHOD_ATTRIBUTE_PURE
  get_id() BOOST_NOEXCEPT_OR_NOTHROW
  { return rtti_get_id( get_node() ); }
};

template<class T>
rtti_node_var<holder<T>::Arity> holder<T>::node;

template<class T>
struct holder<T>::initializer_t::register_one {
  mutable size_t k;

  template<typename P>
  void operator()(P*) const {
    typedef typename get_holder<P>::type U;
    holder::node.__base[k] = U::get_node();
    ++k;
  }
};

template<class T>
holder<T>::initializer_t::initializer_t() {
  holder::node.__arity = Arity;

  register_one reg = { 0 };
  rtti_parents_foreach(reg, (T*)NULL);
}

template<class T>
typename holder<T>::initializer_t
  holder<T>::initializer;

} // namespace holder_

using holder_::get_holder;

}} // namespace rtti::detail

#endif
