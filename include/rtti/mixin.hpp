#ifndef RTTI_MIXIN_HPP
#define RTTI_MIXIN_HPP

#include "rtti/hash/hash.hpp"
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
  constexpr mixin_node()
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
  };

protected:
  mixin() noexcept {
    const_cast<rtti_node&>( detail::rtti_getter::get_node_value(static_cast<Derived&>(*this)) ) = *rtti::static_node<Derived>();
  }
  ~mixin() noexcept {}
};

} // namespace rtti

//@{
//! \brief Enum flags for some optimizations
#define RTTI_FLAGS( declare, abstract, final, static ) \
  ( (declare ? ::rtti::flags::DECLARE : 0) \
  | (abstract? ::rtti::flags::ABSTRACT: 0) \
  | (final   ? ::rtti::flags::FINAL   : 0) \
  | (static  ? ::rtti::flags::STATIC  : 0) )

// ***** externally used macros ***** //
//! \brief Abstract base case
#define ABSTRACT_RTTI( klass, static_max )  \
  public ::rtti::mixin<                     \
    klass                                   \
  , void                                    \
  , RTTI_FLAGS(1, 1, 0, 1)                  \
  , 0                                       \
  >

//! \brief Base case
#define DECLARE_RTTI( klass, static_max )   \
  public ::rtti::mixin<                     \
    klass                                   \
  , void                                    \
  , RTTI_FLAGS(1, 0, 0, 1)                  \
  , static_max                              \
  >

//! \brief Derived case
#define IMPLEMENT_RTTI( klass, parent ) \
  public ::rtti::mixin<                 \
    klass                               \
  , parent                              \
  , RTTI_FLAGS(0, 0, 0, 0)              \
  , __RTTI_HASH_TYPE(klass, 0, 0)       \
  >

//! \brief Static derived case
#define STATIC_RTTI( klass, parent, id )\
  public ::rtti::mixin<                 \
    klass                               \
  , parent                              \
  , RTTI_FLAGS(0, 0, 0, 1)              \
  , __RTTI_HASH_TYPE(klass, 1, id)      \
  >

//! \brief Final case
#define FINAL_RTTI( klass, parent )     \
  public ::rtti::mixin<                 \
    klass                               \
  , parent                              \
  , RTTI_FLAGS(0, 0, 1, 0)              \
  , __RTTI_HASH_TYPE(klass, 0, 0)       \
  >

//! \brief Static final case
#define STATIC_FINAL_RTTI( klass, parent, id ) \
  public ::rtti::mixin<                 \
    klass                               \
  , parent                              \
  , RTTI_FLAGS(0, 0, 1, 1)              \
  , __RTTI_HASH_TYPE(klass, 1, id)      \
  >
//@}

#endif

#include "rtti/holder/getter.ipp"
