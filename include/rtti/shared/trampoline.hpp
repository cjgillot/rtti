#ifndef RTTI_MMETHOD_TRAMPOLINE_HPP
#define RTTI_MMETHOD_TRAMPOLINE_HPP

#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/call_traits.hpp>

#include "rtti/traits.hpp"

namespace rtti {
namespace detail {

template<typename TAG, typename Ret, typename... Types>
struct trampoline {
  typedef Ret func_t(typename boost::call_traits<Types>::param_type...);

  template<typename Over, typename Ret2, typename... Types2>
  struct apply {
    static Over over;
    static Ret call(typename boost::call_traits<Types>::param_type... args)
    { return over.call( rtti::pointer_traits<Types>::template cast<Types2>(args)... ); }
  };
};

template<typename TAG, typename Ret, typename... Types>
template<typename Over, typename Ret2, typename... Types2>
Over trampoline<TAG, Ret, Types...>::apply<Over, Ret2, Types2...>::over;

}} // namespace rtti::detail

#endif
