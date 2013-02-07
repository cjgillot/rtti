#ifndef RTTI_MMETHOD_TRAMPOLINE_HPP
#define RTTI_MMETHOD_TRAMPOLINE_HPP

namespace rtti {
namespace mmethod {
namespace detail {

template<typename TAG, typename Ret, typename... Types>
struct trampoline {
  typedef Ret(*func_t)(Types& ...);

  template<typename Over, typename Ret2, typename... Types2>
  struct apply {
    static Over over;
    static Ret call(Types& ...args)
    { return over.call(static_cast<Types2>(args)...); }
  };
};

template<typename TAG, typename Ret, typename... Types>
template<typename Over, typename Ret2, typename... Types2>
Over trampoline<TAG, Ret, Types...>::apply<Over, Ret2, Types2...>::over;

}}} // namespace rtti::mmethod::detail

#endif
