#ifndef RTTI_HOLDER_HPP
#define RTTI_HOLDER_HPP

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include "rtti/rttifwd.hpp"
#include "rtti/holder/getter.hpp"

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
    static BOOST_CONSTEXPR rtti_node* get_node() { return 0; }
  };
};

//! \brief Root holder with the seed
template<typename Root>
struct root_holder {
  template<bool Static>
  static rtti_type make(std::size_t hash) {
    if(Static)
      return rtti_type(hash);

    static std::size_t current = rtti_getter::traits<Root>::static_max;
    return rtti_type(current++);
  }
};

/*
 * Current implementation :
 *
 * rtti_type is a native unsigned integer,
 * which range allows for at least 2 ^ 16 classes
 * in a hierarchy.
 *
 * A new class id is just its name's string hash value.
 */

//! Arguments must be const-qualified to avoid unnecessary instanciations
template<class T>
struct holder {
private:
  BOOST_STATIC_ASSERT( boost::is_const<T>::value && boost::is_volatile<T>::value
  && "rtti::detail::holder_::holder<> must not be accessed directly" );

  typedef rtti_getter::traits<T> trts;
  typedef typename trts::root    root;
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
  }
  static initializer;

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
  { return get_node()->id; }
};

template<class T>
rtti_node_var<holder<T>::Arity> holder<T>::node;

template<class T>
struct holder<T>::initializer_t::register_one {
  mutable size_t k;

  template<typename U>
  void operator()(U) const {
    holder::node.base[k] = U::get_node();
    ++k;
  }
};

template<class T>
holder<T>::initializer_t::initializer_t() {
  holder::node.id = root_holder<root>::template make<trts::static_>(trts::hash);

  holder::node.arity = Arity;

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
