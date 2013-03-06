#ifndef RTTI_INTERFACE_HPP
#define RTTI_INTERFACE_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include "rtti/holder/getter.ipp"
#include "rtti/hash/hash.hpp"

#include "util/attribute.hpp"
#include "util/compare.hpp"

/*!
 * \brief Macros controlling the is_a behaviour
 * \{
 */
//! Choose whether \ref rtti::is_a should
//! use built-in \c dynamic_cast<> operator,
//! or prefer reimplementing it in terms of \c rtti_node
#define RTTI_IS_A_USE_DYNAMIC_CAST
//! Choose whether \ref rtti::is_a should
//! use \ref rtti::is_exactly_a when the target
//! is flagged as a final class
#define RTTI_IS_A_USE_FINAL
/*!\}*/

/*
 * In this file, the [pure] attribute is
 * overused on purpose to create room for
 * as much optimization as possible.
 */

namespace rtti {

//! \brief RTTI id type
using detail::rtti_type;

//! \brief Function for comparision of RTTI ids
inline util::cmp_t ATTRIBUTE_PURE
rtti_compare( rtti_type a, rtti_type b )
{ return util::compare(a,b); }

//! \brief Function checking exact type
template<class T, class U>
inline bool ATTRIBUTE_PURE
is_exactly_a(const U &x) {
  static_assert( boost::is_same<
    typename RTTI_GETTER::traits<T>::base
  , typename RTTI_GETTER::traits<U>::base
  >::value,
    "is_exactly_a<> should be called"
    "inside an unique hierarchy"
  );

  return rtti::static_id<T>() == rtti::get_id(x);
}

//! \brief Function checking exact type, pointer version
template<class T, class U>
inline bool ATTRIBUTE_PURE ATTRIBUTE_NONNULL(1)
is_exactly_a(const U* x) {
  // reject pointer-to-pointer
  static_assert( ! boost::is_pointer<U>::value, "is_exactly_a<> called with pointer to pointer" );
  typedef typename boost::remove_pointer<T>::type T2;

  // retry without pointer
  return rtti::is_exactly_a<T2, U>( *x );
}

namespace detail {

// T and U shall be const-qualified raw class types (no reference, no pointers)
template<class T, class U>
struct is_a_impl {
  static inline bool ATTRIBUTE_PURE
  apply(const U &x)
  {
#ifdef RTTI_IS_A_USE_DYNAMIC_CAST

    return dynamic_cast<const T*>( &x ) != 0;

#else
    // manual implementation using type nodes

    const ::rtti::rtti_type id = RTTI_TYPE_ID( T );
    const ::rtti::rtti_node* n = RTTI_NODE( x );

    do {
      // got it ?
      if( n->id == id )
        return true;

      // upcast
      n = n->base;
    } while( n );
    return false;

#endif
  }
};

// in case of final target, is_a means is_exactly_a
template<class T, class U>
struct is_a_final {
  static inline bool ATTRIBUTE_PURE
  apply(const U &x)
  {
#ifdef RTTI_IS_A_USE_FINAL

    return is_exactly_a<T, U>( x );

#else

    return is_a_impl<T,U>::apply( x );

#endif
  }
};

// trivial cases
template<class T>
struct is_a_impl<T, T> {
  static inline bool ATTRIBUTE_CONST
  apply(const T &)
  { return true; }
};

template<class T>
struct is_a_final<T, T> {
  static inline bool ATTRIBUTE_CONST
  apply(const T &)
  { return true; }
};

}

/*!
 * \brief Test if a pointer implements a given class
 *
 * This version relies on \c dynamic_cast,
 * and has exactly its complexity.
 */
template<class T, class U>
inline bool ATTRIBUTE_PURE
is_a(const U &x)
{
  STATIC_ASSERT(( boost::is_same<
    typename RTTI_GETTER::traits<T>::base
  , typename RTTI_GETTER::traits<U>::base
  >::value &&
    "is_a should be called"
    "inside an unique hierarchy"
  ));

  // U shall already be a raw class
  typedef typename boost::remove_reference<T>::type T2;
  typedef typename boost::remove_pointer <T2>::type T3;
  typedef typename boost::mpl::if_c<
    RTTI_GETTER::traits<T3>::final_
  , detail::is_a_final<const T3, const U>
  , detail::is_a_impl<const T3, const U>
  >::type impl_t;
  return impl_t::apply( x );
}

template<class T, class U>
inline bool ATTRIBUTE_PURE ATTRIBUTE_NONNULL(1)
is_a(const U* x)
{ return is_a<T>( *x ); }

} // namespace rtti

#endif
