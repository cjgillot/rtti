#ifndef RTTI_MMETHOD_DISPATCH_HPP
#define RTTI_MMETHOD_DISPATCH_HPP

#include "rtti/mmethod/poles.hpp"

#include "rtti/shared/basic.hpp"
#include "rtti/holder/getter.hpp"

namespace rtti { namespace mmethod { namespace detail {

namespace {

/// fetch_poles<>::eval(spec,_,args) loops over args and returns the sum of pole-data
/// equivalent pseudo-code :
/// \code
/// i = 0
/// for each arg,
///   if arg is virtual
///     i += arg->pole->data
/// \endcode
template<typename Tag, std::size_t BTS>
struct fetch_poles {
  template<std::size_t J, typename First2, typename... Types2>
  static std::uintptr_t ATTRIBUTE_PURE eval(First2&& arg, Types2&&... args) {
    std::uintptr_t rh = 0;

    if( BTS & 1 ) {
      auto& map = Tag::template poles<J>::array;
      rh = map.fetch_pole( ::rtti::get_node(arg) );
    }

    using next = fetch_poles<Tag, BTS/2>;
    return rh + next::template eval<J+1>(std::forward<Types2>(args)...);
  }
};
template<typename Tag>
struct fetch_poles<Tag, 0> {
  template<std::size_t J, typename... Types>
  static std::uintptr_t ATTRIBUTE_CONST eval(Types&&...)
  { return 0; }
};

template<std::size_t Arity, typename Tag, std::size_t BTS>
struct fetch_invoker {
  static invoker_t ATTRIBUTE_PURE eval(std::uintptr_t spec) {
    return Tag::invoker_table[ spec >> 1 ];
  }
};
template<typename Tag, std::size_t BTS>
struct fetch_invoker<1, Tag, BTS> {
  static invoker_t ATTRIBUTE_PURE eval(std::uintptr_t spec) {
    return (invoker_t)spec;
  }
};

} // namespace <>

/// main dispatch function
template<typename Tag, typename Ret>
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
  invoker_t ATTRIBUTE_PURE fetch(Types2&&... args) const {
    constexpr std::size_t arity = Tag::traits::vsize;
    constexpr std::size_t btset = Tag::traits::type_bitset;

    using fpoles = fetch_poles<Tag, btset>;
    std::uintptr_t spec = fpoles::template eval<0>( std::forward<Types2>(args)... );

    using finvoker = fetch_invoker<arity, Tag, btset>;
    return finvoker::eval(spec);
  }

  template<typename... Types2>
  Ret call(Types2&&... args) const {
    invoker_t f = this->fetch( std::forward<Types2>(args)... );

    typedef typename Tag::traits::trampoline::func_t* func_t;
    return reinterpret_cast<func_t>(f)( std::forward<Types2>(args)...);
  }
};

}}} // rtti::mmethod::detail

#endif
