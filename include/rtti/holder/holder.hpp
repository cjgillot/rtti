#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include "rtti/rttifwd.hpp"
#include "rtti/holder/getter.hpp"

#include "rtti/holder/node.hpp"

#include <boost/assert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>

namespace rtti {
namespace detail {
namespace holder_ {

template<typename> struct holder;

//! \brief Grant access to the holder
struct get_holder {
  template<class T>
  struct apply {
    typedef holder<T const volatile> type;
  };
};

//! \brief Placeholder struct used for hierarchy base
template<>
struct get_holder::apply<const volatile void> {
  struct type {
    static BOOST_CONSTEXPR rtti_node* get_node() { return NULL; }
  };
};

//! Arguments must be const-qualified to avoid unnecessary instanciations
template<class T>
struct holder {
private:
  BOOST_STATIC_ASSERT( boost::is_const<T>::value && boost::is_volatile<T>::value
  && "rtti::detail::holder_::holder<> must not be accessed directly" );

  typedef rtti_getter::traits<T> trts;
  typedef typename trts::parents parents;
  
  typedef typename boost::mpl::transform<
    parents
  , get_holder
  >::type sholders;
  enum {
    Arity = boost::mpl::size<parents>::value
  };

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
  ATTRIBUTE_PURE
  get_node() BOOST_NOEXCEPT_OR_NOTHROW
  { initializer.touch(); return reinterpret_cast<rtti_node*>(&node); }

  static inline
  rtti_type
  ATTRIBUTE_PURE
  get_id() BOOST_NOEXCEPT_OR_NOTHROW
  { return rtti_get_id( get_node() ); }
};

template<class T>
rtti_node_var<holder<T>::Arity> holder<T>::node;

template<class T>
struct holder<T>::initializer_t::register_one {
  mutable size_t k;

  template<typename U>
  void operator()(U) const {
    holder::node.__base[k] = U::get_node();
    ++k;
  }
};

template<class T>
holder<T>::initializer_t::initializer_t() {
  holder::node.__arity = Arity;

  register_one reg = { 0 };
  boost::mpl::for_each<
    sholders
  >( reg );
}

template<class T>
typename holder<T>::initializer_t
  holder<T>::initializer;

} // namespace holder_

using holder_::get_holder;

}} // namespace rtti::detail

#endif

// #include "rtti/holder.ipp"
