#ifndef RTTI_MMETHOD_TRAMPOLINE_HPP
#define RTTI_MMETHOD_TRAMPOLINE_HPP

#include "rtti/traits.hpp"
#include "rtti/shared/tags.hpp"

namespace rtti {
namespace detail {

template<typename TAG, typename Ret, typename... Types>
struct trampoline {
  typedef Ret return_type;
  typedef Ret sig_t(typename tags::unwrap<Types>::arg_type...);

  template<typename Over, typename Ret2, typename... Types2>
  struct apply {
    static Over over;
    static Ret call(typename tags::unwrap<Types>::arg_type... args)
    { return over.call( rtti::pointer_traits<Types>::template cast<Types2>(args)... ); }
  };

  template<typename Ret2, typename... Types2, typename Func>
  static std::function<sig_t> functor(Func&& f) {
    return [f](typename tags::unwrap<Types>::arg_type... args) -> Ret
    { return f( rtti::pointer_traits<Types>::template cast<Types2>(args)... ); };
  }
};

template<typename TAG, typename Ret, typename... Types>
template<typename Over, typename Ret2, typename... Types2>
Over trampoline<TAG, Ret, Types...>::apply<Over, Ret2, Types2...>::over;

}} // namespace rtti::detail

#endif
