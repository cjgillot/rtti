#ifndef RTTI_DMETHOD_DISPATCH_HPP
#define RTTI_DMETHOD_DISPATCH_HPP

#include "rtti/dynamic/poles.hpp"

#include "rtti/shared/basic.hpp"
#include "rtti/holder/getter.hpp"

#include <functional>

namespace rtti { namespace dmethod { namespace detail {

functor_t ATTRIBUTE_PURE lookup(
  std::size_t arity
, invoker_table_type& table
, std::uintptr_t* spec
, rtti_node const** hiers
) noexcept;

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
  template<std::size_t J, typename First, typename... Types>
  static void eval(std::uintptr_t* m, rtti_node const** h, First&& arg0, Types&&... args) {
    if( BTS & 1 ) {
      detail::poles_map_type& map = Tag::template poles<J>::array;

#if MMETHOD_USE_THREAD
      util::stw_lock::fetch_guard lock { map.lock };
#endif

      *h = ::rtti::get_node(arg0);
      *m = map.mem.fetch_pole(*h);
      ++h; ++m;
    }

    using next = fetch_poles<Tag, BTS/2>;
    next::template eval<J+1>( m, h, std::forward<Types>(args)... );
  }
};
template<typename Tag>
struct fetch_poles<Tag, 0> {
  template<std::size_t J, typename... Types>
  static void eval(Types&&... /*args*/) {}
};

template<std::size_t Arity, typename Tag, std::size_t BTS>
struct fetch_invoker {
  static functor_t ATTRIBUTE_PURE eval(std::uintptr_t* spec, rtti_node const** hiers) {
    return lookup( Arity, Tag::invoker_table, spec, hiers );
  }
};
template<typename Tag, std::size_t BTS>
struct fetch_invoker<1, Tag, BTS> {
  static functor_t ATTRIBUTE_PURE eval(std::uintptr_t* spec, rtti_node const**) {
    return (functor_t)spec[0];
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
  functor_t ATTRIBUTE_PURE fetch(Types2&&... args) const {
    constexpr std::size_t arity = Tag::traits::vsize;
    constexpr std::size_t btset = Tag::traits::type_bitset;

    std::uintptr_t spec [ arity ];
    rtti_node const* hiers [ arity ];

    using fpoles = fetch_poles<Tag, btset>;
    fpoles::template eval<0>( spec, hiers, std::forward<Types2>(args)... );

    using finvoker = fetch_invoker<arity, Tag, btset>;
    return finvoker::eval(spec, hiers);
  }

  template<typename... Types2>
  Ret call(Types2&&... args) const {
    functor_t f = this->fetch( std::forward<Types2>(args)... );

    if(!f)
      _rtti_bad_dispatch();

    typedef typename Tag::traits::trampoline::func_t sig_t;
    typedef std::function<sig_t> func_t;
    return (*static_cast<func_t*>(f))( std::forward<Types2>(args)... );
  }

  template<typename... K, typename F>
  void insert(F&& f);

  template<typename... K>
  void retract();
};

}}} // rtti::mmethod::detail

#include "rtti/dynamic/dynamic.hpp"

#endif
