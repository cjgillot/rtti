#ifndef RTTI_MIXIN_HPP
#define RTTI_MIXIN_HPP

// #include "rtti/hash/hash.hpp"
#include "rtti/holder/holder.hpp"
#include "rtti/holder/getter.hpp"

#include <type_traits>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/transform.hpp>

namespace rtti {

namespace flags {
enum {
  DECLARE  = 1 << 0
, ABSTRACT = 1 << 1
, FINAL    = 1 << 2
, STATIC   = 1 << 3
};
} // namespace flags

namespace detail {

template<bool> struct mixin_node {};
template<> struct mixin_node<true> {
  rtti::rtti_node const rtti_node_value;

protected:
  mixin_node()
  : rtti_node_value(rtti_node_value) {}
};

template<typename Root>
struct has_root {
  template<typename T>
  struct apply
  : boost::is_same< typename rtti_getter::traits<T>::root, Root >
  {};
};

template<bool Declare, typename Derived, typename Supers, std::size_t Hash>
struct mixin_helper {
private:
  typedef typename boost::mpl::front<Supers>::type Super0;
  typedef rtti_getter::traits<Super0> Traits;

public:
  constexpr static rtti_type static_max = Traits::static_max;
  constexpr static rtti_type hash       { Hash };
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
template<typename D, typename S, std::size_t Max>
struct mixin_helper<true, D, S, Max> {
  constexpr static rtti_type static_max { Max };
  constexpr static rtti_type hash       { 0 };

  typedef D const volatile      self;
  typedef self                  root;
  typedef boost::mpl::vector<>  parents;
};

} // namespace detail

template<
  typename Derived
, typename Supers
, unsigned char Flags
, std::size_t Hash
>
struct mixin
: private detail::mixin_node<Flags & flags::DECLARE> {

  friend class detail::rtti_getter;
  friend mixin rtti_get_mixin(Derived const volatile&) {
    // dummy body -> avoid gcc non-template-friend warning
    return std::declval<mixin>();
  }

private:
  // structure used by RTTI_GETTER
  struct rtti_traits {
  private:
    typedef detail::mixin_helper<Flags & rtti::flags::DECLARE, Derived, Supers, Hash> helper;

  public:
    typedef typename helper::self    self;
    typedef typename helper::parents parents;
    typedef typename helper::root    root;
    
    static const bool abstract_ = Flags & rtti::flags::ABSTRACT;
    static const bool static_   = Flags & rtti::flags::STATIC;
    static const bool final_    = Flags & rtti::flags::FINAL;

    constexpr static rtti_type static_max = helper::static_max;
    constexpr static rtti_type hash       = helper::hash;

    BOOST_STATIC_ASSERT( hash < static_max );
  };

protected:
  mixin() noexcept {
    const_cast<rtti_node&>( detail::rtti_getter::get_node_value(static_cast<Derived&>(*this)) ) = *rtti::static_node<Derived>();
  }
  ~mixin() noexcept {}
};

//@{
//! \brief Enum flags for some optimizations
#define RTTI_FLAGS( declare, abstract, final, static ) \
  ( (declare ? ::rtti::flags::DECLARE : 0) \
  | (abstract? ::rtti::flags::ABSTRACT: 0) \
  | (final   ? ::rtti::flags::FINAL   : 0) \
  | (static  ? ::rtti::flags::STATIC  : 0) )

//! \brief Base case
template<typename klass, std::size_t static_max = 256>
struct base_rtti
: public mixin<
  klass, void const volatile
, flags::DECLARE | flags::STATIC
, static_max
> {};

//! \brief Abstract base case
template<typename klass, std::size_t static_max = 256>
struct abstract_base_rtti
: public mixin<
  klass, void const volatile
, flags::DECLARE | flags::STATIC | flags::ABSTRACT
, static_max
> {};

template<typename klass, typename parents>
struct implement_rtti
: public mixin<
  klass, parents
, 0
, 0
> {};

template<typename klass, typename parents, std::size_t id>
struct static_rtti
: public mixin<
  klass, parents
, flags::STATIC
, id
> {};

template<typename klass, typename parents>
struct final_rtti
: public mixin<
  klass, parents
, flags::FINAL
, 0
> {};

template<typename klass, typename parents, std::size_t id>
struct static_final_rtti
: public mixin<
  klass, parents
, flags::FINAL | flags::STATIC
, id
> {};

//@}

} // namespace rtti

#endif

#include "rtti/holder/getter.ipp"
