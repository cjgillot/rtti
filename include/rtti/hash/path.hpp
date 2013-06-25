#ifndef RTTI_HASH_PATH_HPP
#define RTTI_HASH_PATH_HPP

#include "rtti/interface.hpp"
#include "rtti/shared/mpl.hpp"

namespace rtti { namespace hash {

/*!\name Hash path construction
 *
 * These functions construct a list containing the hashes, used by the external tool.
 * \{
 */
//@{
///\brief List node
template<size_t H, typename Base>
struct path_node {};
///\brief List end tag
struct path_end {};

namespace detail {

struct hash {
  template<typename T>
  struct apply {
    typedef typename mpl::remove_all<T>::type RawT;
    constexpr static rtti_type value = rtti::type_hash<RawT>();
    typedef apply type;
  };
};

} // namespace detail

/*!\brief List construction
 * Recursively constructs the list,
 * starting from \c T,
 * and upcasting,
 * until the base class.
 */
template<typename T>
struct make_path {
  typedef typename mpl::remove_all<T>::type RawT;
  typedef rtti::detail::rtti_getter::traits<RawT> Traits;
  typedef path_node<
    detail::hash::template apply<T>::value
  , typename make_path<typename Traits::super>::type
  > type;
};
/*!\brief List construction
 * This specialization is called by <tt>mmethod_hpath_make< </it>base class</it> ></tt>
 */
template<>
struct make_path<const volatile void> {
  typedef path_end type;
};
//@}
/*!\}*/

}} // namespace rtti

#endif
