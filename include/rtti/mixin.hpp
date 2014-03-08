#ifndef RTTI_MIXIN_HPP
#define RTTI_MIXIN_HPP

// #include "rtti/hash/hash.hpp"
#include "rtti/holder/holder.hpp"
#include "rtti/holder/getter.hpp"

#include <type_traits>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>

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

template<bool Declare, typename Derived, typename Super, std::size_t Hash>
struct mixin_helper {
private:
  using Traits = rtti_getter::traits<Super>;
  
public:
  constexpr static rtti_type static_max = Traits::static_max;
  constexpr static rtti_type hash       { Hash };
  using root = typename Traits::root;
};
template<typename D, typename S, std::size_t Max>
struct mixin_helper<true, D, S, Max> {
  constexpr static rtti_type static_max { Max };
  constexpr static rtti_type hash       { 0 };
  using root = D;
};

} // namespace detail

template<
  typename Derived
, typename Super
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
    typedef detail::mixin_helper<Flags & rtti::flags::DECLARE, Derived, Super, Hash> helper;

  public:
    typedef Derived const volatile self;
    typedef Super const volatile super;
    typedef typename helper::root const volatile root;
    
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
  klass, void,
  flags::DECLARE | flags::STATIC
, static_max
> {};

//! \brief Abstract base case
template<typename klass, std::size_t static_max = 256>
struct abstract_base_rtti
: public mixin<
  klass, void
, flags::DECLARE | flags::ABSTRACT
, static_max
> {};

template<typename klass, typename parent>
struct implement_rtti
: public mixin<
  klass, parent
, 0
, 0
> {};

template<typename klass, typename parent, std::size_t id>
struct static_rtti
: public mixin<
  klass, parent
, flags::STATIC
, id
> {};

template<typename klass, typename parent>
struct final_rtti
: public mixin<
  klass, parent
, flags::FINAL
, 0
> {};

template<typename klass, typename parent, std::size_t id>
struct static_final_rtti
: public mixin<
  klass, parent
, flags::FINAL | flags::STATIC
, id
> {};

//@}

} // namespace rtti

#endif

#include "rtti/holder/getter.ipp"
