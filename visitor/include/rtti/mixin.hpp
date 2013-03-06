#ifndef RTTI_MIXIN_HPP
#define RTTI_MIXIN_HPP

#include "rtti/holder.hpp"
#include "rtti/getter.hpp"
#include "rtti/hash.hpp"

#include <type_traits>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>

namespace rtti {

namespace flags {
enum {
  DECLARE_MASK  = 1 << 0
, ABSTRACT_MASK = 1 << 1
, FINAL_MASK    = 1 << 2
, STATIC_MASK   = 1 << 3
};
} // namespace flags

namespace detail {

template<bool> struct mixin_node {};
template<> struct mixin_node<true> { rtti::rtti_node const* rtti_node_pointer; };

template<bool, typename Derived, typename Super, std::size_t Hash>
struct mixin_helper {
  enum {
    static_max = RTTI_GETTER::traits<Super>::static_max
  , hash = Hash
  };
  using base = typename RTTI_GETTER::traits<Super>::base;
};
template<typename D, typename S, std::size_t Max>
struct mixin_helper<true, D, S, Max> {
  enum {
    static_max = Max
  , hash = 0
  };
  using base = D;
};

} // namespace detail


template<
  typename Derived
, typename Super
, unsigned char Flags
, std::size_t Hash
>
struct mixin
: private detail::mixin_node<Flags & flags::DECLARE_MASK> {

  friend class RTTI_GETTER;
  friend mixin rtti_get_mixin(Derived const&) {
    // dummy body -> avoid gcc non-template-friend warning
    return std::declval<mixin>();
  }

private:
  // structure used by RTTI_GETTER
  struct rtti_traits {
  private:
    typedef detail::mixin_helper<Flags & rtti::flags::DECLARE_MASK, Derived, Super, Hash> helper;

  public:
    typedef Derived const self;
    typedef Super const super;
    typedef typename helper::base const base;

    enum {
      abstract_= Flags & rtti::flags::ABSTRACT_MASK
    , final_   = Flags & rtti::flags::FINAL_MASK
    , static_  = Flags & rtti::flags::STATIC_MASK

    , static_max = helper::static_max
    , hash = helper::hash
    };
  };

protected:
  mixin() noexcept {
    const_cast<rtti_node const*&>( RTTI_GETTER::get_node_pointer(static_cast<Derived&>(*this)) ) = rtti::static_node<Derived>();
  }
  ~mixin() noexcept {}
};

} // namespace rtti

//@{
//! \brief Enum flags for some optimizations
#define RTTI_FLAGS( declare, abstract, final, static ) \
  ( (declare ? ::rtti::flags::DECLARE_MASK : 0) \
  | (abstract? ::rtti::flags::ABSTRACT_MASK: 0) \
  | (final   ? ::rtti::flags::FINAL_MASK   : 0) \
  | (static  ? ::rtti::flags::STATIC_MASK  : 0) )

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

#include "rtti/creator.tpp"
#include "rtti/holder.tpp"

#include "rtti/getter.ipp"
