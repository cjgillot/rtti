#ifndef RTTI_MMETHOD_DISPATCH_HPP
#define RTTI_MMETHOD_DISPATCH_HPP

#include "rtti/mmethod/basic.hpp"
#include "rtti/mmethod/poles.hpp"

#include "rtti/getter.hpp"

namespace rtti { namespace mmethod { namespace detail {

namespace {

/// fetch_poles<>::eval(spec,_,args) loops over args and blits pole-data into [spec]
/// equivalent pseudo-code :
/// \code
/// i = 0
/// for each arg,
///   if arg is virtual
///     spec[i] = arg->pole->data
///     ++i
/// \endcode
/// The spec array is then used for dispatch computation
template<std::size_t I, std::size_t J, typename Tag, typename... Types>
struct fetch_poles;
template<std::size_t I, std::size_t J, typename Tag, typename First, typename... Types>
struct fetch_poles<I, J, Tag, tags::virtual_<First>, Types...> {
  template<typename First2, typename... Types2>
  static std::uintptr_t eval(First2&& arg, Types2&&... args) {
    std::uintptr_t rh = detail::fetch_pole(
      Tag::template poles<J>::array,
      ::rtti::get_node(arg)
    );
    return rh + fetch_poles<I+1, J+1, Tag, Types...>::eval(std::forward<Types2>(args)...);
  }
};
template<std::size_t I, std::size_t J, typename Tag, typename First, typename... Types>
struct fetch_poles<I, J, Tag, tags::static_<First>, Types...> {
  template<typename First2, typename... Types2>
  static std::uintptr_t eval(First2&&, Types2&&... args) {
    return fetch_poles<I, J+1, Tag, Types...>::eval(std::forward<Types2>(args)...);
  }
};
template<std::size_t I, std::size_t J, typename Tag>
struct fetch_poles<I, J, Tag> {
  static std::uintptr_t eval()
  { return 0; }
};

template<std::size_t Arity, typename Tag, typename... Types>
struct fetch_invoker {
  template<typename... Types2>
  static invoker_t eval(Types2&&... args) {
    std::uintptr_t spec = fetch_poles<0, 0, Tag, Types...>::eval( std::forward<Types2>(args)...);
    return Tag::invoker_table[ spec/2 ];
  }
};
template<typename Tag, typename... Types>
struct fetch_invoker<1, Tag, Types...> {
  template<typename... Types2>
  static invoker_t eval(Types2&&... args) {
    std::uintptr_t spec = fetch_poles<0, 0, Tag, Types...>::eval(std::forward<Types2>(args)...);
    return (invoker_t)spec;
  }
};

} // namespace <>

/// main dispatch function
template<typename Tag, typename Ret, typename... Types>
struct dispatch {
  dispatch() {
    struct initializer_t {
      initializer_t() { Tag::do_initialize(); };
      void touch() {}
    }
    static initializer;
    initializer.touch();
  }

  template<typename... Types2>
  Ret call(Types2&&... args) const {
    // actual work
    enum { arity = Tag::vsize };

    invoker_t f = fetch_invoker<arity, Tag, Types...>::eval(std::forward<Types2>(args)...);
    typedef typename Tag::trampoline::func_t func_t;
    return reinterpret_cast<func_t>(f)(args...);
  }
};

}}} // rtti::mmethod::detail

#endif
