#ifndef RTTI_MMETHOD_REGBASE_HPP
#define RTTI_MMETHOD_REGBASE_HPP

#include <cstdint>

#include "rtti/rttifwd.hpp"

#include "rtti/getter.hpp"
#include "rtti/holder.hpp"

#include "rtti/mmethod/basic.hpp"
#include "rtti/mmethod/dispatch.hpp"

#include <boost/mpl/size_t.hpp>

namespace rtti {
namespace mmethod {

template<typename TAG, size_t ARITY, size_t FULLARITY, size_t TAGS>
struct mmethod_declare { enum { sz = 5 + TAGS }; };

template<typename TAG, size_t ARITY, size_t FULLARITY, typename HASHES>
struct mmethod_implement { enum { sz = 5 + sizeof(HASHES) }; };

/*!\name Hash path construction
 *
 * These functions construct a list containing the hashes, used by the external tool.
 * \{
 */
//@{
///\brief List node
template<size_t H, typename Base>
struct mmethod_hpath {};
///\brief List end tag
struct mmethod_hpath_end {};

namespace detail {

struct hash {
  template<typename T>
  struct apply {
    typedef typename type_traits::remove_all<T>::type RawT;
    constexpr static rtti_type value = rtti::type_hash<RawT>();
    typedef apply type;
  };
};

/*!\brief List construction
 * Recursively constructs the list,
 * starting from \c T,
 * and upcasting,
 * until the base class.
 */
template<typename T>
struct mmethod_hpath_make {
  typedef typename type_traits::remove_all<T>::type RawT;
  typedef RTTI_GETTER::traits<RawT> Traits;
  typedef mmethod_hpath<
    hash::template apply<T>::value
  , typename mmethod_hpath_make<typename Traits::super>::type
  > type;
};
/*!\brief List construction
 * This specialization is called by <tt>mmethod_hpath_make< </it>base class</it> ></tt>
 */
template<>
struct mmethod_hpath_make<const volatile void> {
  typedef mmethod_hpath_end type;
};
//@}
/*!\}*/

struct invalid_node {
  typedef invalid_node type;
  static const rtti_node node;
};

template<typename Tag, typename Ret, typename... Args>
struct make_declare_helper;

template<typename Tag, typename Over, typename Ret, typename... Args>
struct make_implement_helper;

template<typename Tag, typename Ret, typename... Args>
struct make_hashes_helper;

} // namespace detail

}} // namespace rtti::mmethod

#endif
