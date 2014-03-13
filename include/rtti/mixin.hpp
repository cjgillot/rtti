#ifndef RTTI_MIXIN_HPP
#define RTTI_MIXIN_HPP

#include "rtti/holder/holder.hpp"
#include "rtti/holder/getter.hpp"

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>

namespace rtti {
namespace detail {

struct mixin_node {
  rtti::rtti_node const* rtti_node_value;
};

} // namespace detail

template<
  typename Derived
, typename Supers
, typename Declare
>
struct mixin
: private virtual detail::mixin_node {

private:
  // parent classes manipulation
  typedef typename boost::mpl::transform<
    Supers
  , boost::add_cv<boost::mpl::_>
  >::type parents;

  BOOST_STATIC_CONSTANT(std::size_t, arity = boost::mpl::size<parents>::value);
  struct arity_type { unsigned char __dummy [ arity ]; };
  BOOST_STATIC_ASSERT( sizeof(arity_type) == arity );

public:
  friend class detail::rtti_getter;
  friend mixin const& rtti_get_mixin(Derived const& d) {
    return static_cast<mixin const&>(d);
  }
  friend arity_type rtti_parents_size(Derived const volatile*) {
    // dummy body : we don't want any call to this
    return boost::declval<arity_type>();
  }
  template<typename F>
  friend void rtti_parents_foreach(F f, Derived const volatile*) {
    boost::mpl::for_each<
      parents,
      boost::add_pointer<boost::mpl::_>
    >(f);
  }

protected:
  mixin() BOOST_NOEXCEPT_OR_NOTHROW {
    this->detail::mixin_node::rtti_node_value = rtti::static_node<Derived>();
  }
  ~mixin() BOOST_NOEXCEPT_OR_NOTHROW {}
};

//@{
//! \brief Base case
template<typename klass>
struct base_rtti
: public mixin<
  klass, boost::mpl::vector<>
, boost::true_type
> {};

template<typename klass, typename parents>
struct implement_rtti
: public mixin<
  klass, parents
, boost::false_type
> {};

//@}

} // namespace rtti

#endif

#include "rtti/holder/getter.ipp"
