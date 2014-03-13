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
#include <boost/mpl/transform.hpp>

namespace rtti {

namespace flags {
enum {
  DECLARE  = 1 << 0
, ABSTRACT = 1 << 1
, FINAL    = 1 << 2
};
} // namespace flags

namespace detail {

struct mixin_node {
  rtti::rtti_node const* rtti_node_value;
};

template<typename Root>
struct has_root {
  template<typename T>
  struct apply
  : boost::is_same< typename rtti_getter::traits<T>::root, Root >
  {};
};

template<bool Declare, typename Derived, typename Supers>
struct mixin_helper {
private:
  typedef typename boost::mpl::front<Supers>::type Super0;
  typedef rtti_getter::traits<Super0> Traits;

public:
  typedef typename Traits::root const volatile root;

  typedef typename boost::mpl::transform<
    Supers
  , boost::add_cv<boost::mpl::_>
  >::type parents;

  typedef Derived const volatile self;

private:
  BOOST_STATIC_ASSERT((boost::mpl::fold<
    Supers
  , boost::true_type
  , boost::mpl::and_<
      boost::mpl::_1,
      boost::mpl::bind< has_root<root>, boost::mpl::_2 >
    >
  >::type::value
    && "hierarchy must have a unique root class !"
  ));
};
template<typename D, typename S>
struct mixin_helper<true, D, S> {
  typedef D const volatile      self;
  typedef self                  root;
  typedef boost::mpl::vector<>  parents;
};

} // namespace detail

template<
  typename Derived
, typename Supers
, typename Flags
>
struct mixin
: private virtual detail::mixin_node {

  friend class detail::rtti_getter;
  friend mixin const& rtti_get_mixin(Derived const& d) {
    return static_cast<mixin const&>(d);
  }

private:
  // structure used by RTTI_GETTER
  struct rtti_traits {
  private:
    typedef detail::mixin_helper<Flags::value & rtti::flags::DECLARE, Derived, Supers> helper;

  public:
    typedef typename helper::self    self;
    typedef typename helper::parents parents;
    typedef typename helper::root    root;

    static const bool abstract_ = Flags::value & rtti::flags::ABSTRACT;
    static const bool final_    = Flags::value & rtti::flags::FINAL;
  };

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
  klass, void const volatile
, boost::mpl::size_t< flags::DECLARE >
> {};

//! \brief Abstract base case
template<typename klass>
struct abstract_base_rtti
: public mixin<
  klass, void const volatile
, boost::mpl::size_t< flags::DECLARE | flags::ABSTRACT >
> {};

template<typename klass, typename parents>
struct implement_rtti
: public mixin<
  klass, parents
, boost::mpl::size_t< 0 >
> {};

template<typename klass, typename parents>
struct final_rtti
: public mixin<
  klass, parents
, boost::mpl::size_t< flags::FINAL >
> {};

//@}

} // namespace rtti

#include <boost/typeof/typeof.hpp>
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TEMPLATE(rtti::mixin, 3)

#endif

#include "rtti/holder/getter.ipp"
