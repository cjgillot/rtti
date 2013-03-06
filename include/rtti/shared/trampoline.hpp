#ifndef RTTI_MMETHOD_TRAMPOLINE_HPP
#define RTTI_MMETHOD_TRAMPOLINE_HPP

#include <boost/type_traits/is_virtual_base_of.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Out>
struct unsafe_casting {
  struct nonvirtual {
    template<typename In>
    static Out eval(In& in)
    { return static_cast<Out>(in); }
  };
  struct dynamic {
    template<typename In>
    static Out eval(In& in)
    { return dynamic_cast<Out>(in); }
  };

  template<typename In>
  static Out eval(In& in) {
    constexpr bool dyn = boost::is_virtual_base_of<Out, In>::value;
    using impl = typename std::conditional<dyn, dynamic, nonvirtual>::type;
    return impl::eval(in);
  }
};

template<typename TAG, typename Ret, typename... Types>
struct trampoline {
  typedef Ret(*func_t)(Types& ...);

  template<typename Over, typename Ret2, typename... Types2>
  struct apply {
    static Over over;
    static Ret call(Types& ...args)
    { return over.call( unsafe_casting<Types2>::eval(args)... ); }
  };
};

template<typename TAG, typename Ret, typename... Types>
template<typename Over, typename Ret2, typename... Types2>
Over trampoline<TAG, Ret, Types...>::apply<Over, Ret2, Types2...>::over;

}}} // namespace rtti::mmethod::detail

#endif
