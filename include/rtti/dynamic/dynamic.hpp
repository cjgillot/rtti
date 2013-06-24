#ifndef RTTI_DMETHOD_DYNAMIC_HPP
#define RTTI_DMETHOD_DYNAMIC_HPP

#include "rtti/dynamic/common.hpp"
#include "rtti/dynamic/dispatch.hpp"

#include "rtti/dynamic/declare.hpp"
#include "rtti/dynamic/declare.tpp"

namespace rtti { namespace dmethod {

namespace detail {

void init_pole      (detail::poles_map_type& map);
void init_pole_unary(detail::poles_map_type& map);
void init_table(std::size_t arity, detail::invoker_table_type& tbl);

std::uintptr_t insert_pole       (poles_map_type& map, rtti_node const* hier);
void           insert_pole_unary (poles_map_type& map, rtti_node const* hier, functor_t inv);
void           insert (std::size_t arity, invoker_table_type& table, functor_t inv, std::uintptr_t* spec, rtti_node const** hiers);

std::uintptr_t retract_pole      (poles_map_type& map, rtti_node const* hier);
functor_t      retract_pole_unary(poles_map_type& map, rtti_node const* hier);
functor_t      retract(std::size_t arity, invoker_table_type& table, std::uintptr_t* spec, rtti_node const** hiers);

} // namespace detail

namespace {

template<typename Tag, std::size_t BTS>
struct init_poles {
  template<std::size_t J, std::size_t arity>
  static void eval() {
    if( BTS & 1 ) {
      detail::poles_map_type& map = Tag::template poles<J>::array;

      if( arity == 1 )
        detail::init_pole_unary(map);
      else
        detail::init_pole(map);
    }

    using next = init_poles<Tag, BTS/2>;
    next::template eval<J+1, arity>();
  }
};
template<typename Tag>
struct init_poles<Tag, 0> {
  template<std::size_t, std::size_t>
  static void eval() {}
};

} // namespace <>

template<typename Tag>
detail::invoker_table_type register_base<Tag>::invoker_table;

template<typename Tag>
template<std::size_t J>
detail::poles_map_type register_base<Tag>::poles<J>::array;

template<typename Tag>
void register_base<Tag>::do_initialize() {
  constexpr std::size_t arity = Tag::traits::vsize;
  constexpr std::size_t btset = Tag::traits::type_bitset;

  using spoles = init_poles<Tag, btset>;
  spoles::template eval<0, arity>();

  if( arity != 1 )
    detail::init_table(arity, Tag::invoker_table);
}

namespace detail {

namespace {

template<typename Tag, std::size_t BTS>
struct save_poles {
  template<std::size_t J, typename K0, typename... Ks>
  static void eval(std::uintptr_t* m, rtti_node const** h, mpl::mplpack<K0, Ks...>) {
    if( BTS & 1 ) {
      *h = ::rtti::static_node< typename mpl::remove_all<K0>::type >();
      *m = insert_pole( Tag::template poles<J>::array, *h );
      ++h; ++m;
    }

    using next = save_poles<Tag, BTS/2>;
    next::template eval<J+1>(m, h, mpl::mplpack<Ks...>());
  }
};
template<typename Tag>
struct save_poles<Tag, 0> {
  template<std::size_t, typename... P>
  static void eval(P&& .../*args*/) {}
};

template<typename Tag, std::size_t BTS>
struct save_poles_unary {
  template<std::size_t J, typename K0, typename... Ks>
  static void eval(functor_t inv, mpl::mplpack<K0, Ks...>) {
    if( BTS & 1 ) {
      auto node = ::rtti::static_node< typename mpl::remove_all<K0>::type >();
      insert_pole_unary( Tag::template poles<J>::array, node, inv );
    }

    using next = save_poles_unary<Tag, BTS/2>;
    next::template eval<J+1>(inv, mpl::mplpack<Ks...>());
  }
};
template<typename Tag>
struct save_poles_unary<Tag, 0> {
  template<std::size_t, typename... P>
  static void eval(P&& .../*args*/) {}
};

} // namespace <>

template<typename Tag, typename Ret>
template<typename... K, typename F>
void dispatch<Tag,Ret>::insert(F&& f) {
  constexpr std::size_t arity = Tag::traits::vsize;
  constexpr std::size_t btset = Tag::traits::type_bitset;

  typedef typename Tag::traits::trampoline::func_t sig_t;
  typedef std::function<sig_t> func_t;
  functor_t inv = new func_t{ Tag::traits::trampoline::template functor<Ret,K...>(f) };

  if(arity == 1) {
    using spoles = save_poles_unary<Tag, btset>;
    spoles::template eval<0>( inv, mpl::mplpack<K...>() );
  }
  else {
    std::uintptr_t spec [ arity ];
    rtti_node const* hiers [ arity ];

    using spoles = save_poles<Tag, btset>;
    spoles::template eval<0>( spec, hiers, mpl::mplpack<K...>() );
    detail::insert( arity, Tag::invoker_table, inv, spec, hiers );
  }
}

namespace {

template<typename Tag, std::size_t BTS>
struct rem_poles {
  template<std::size_t J, typename K0, typename... Ks>
  static void eval(std::uintptr_t* m, rtti_node const** h, mpl::mplpack<K0, Ks...>) {
    if( BTS & 1 ) {
      *h = ::rtti::static_node< typename mpl::remove_all<K0>::type >();
      *m = retract_pole( Tag::template poles<J>::array, *h );
      ++h; ++m;
    }

    using next = rem_poles<Tag, BTS/2>;
    next::template eval<J+1>(m, h, mpl::mplpack<Ks...>());
  }
};
template<typename Tag>
struct rem_poles<Tag, 0> {
  template<std::size_t, typename... P>
  static void eval(P&& ...args) {}
};

template<typename Tag, std::size_t BTS>
struct rem_poles_unary {
  template<std::size_t J, typename K0, typename... Ks>
  static functor_t eval(mpl::mplpack<K0, Ks...>) {
    if( BTS & 1 ) {
      auto node = ::rtti::static_node< typename mpl::remove_all<K0>::type >();
      return retract_pole_unary( Tag::template poles<J>::array, node );
    }

    using next = rem_poles_unary<Tag, BTS/2>;
    return next::template eval<J+1>(mpl::mplpack<Ks...>());
  }
};
template<typename Tag>
struct rem_poles_unary<Tag, 0> {
  template<std::size_t, typename... P>
  static functor_t eval(P&& ...args) { return nullptr; }
};

} // namespace <>

template<typename Tag, typename Ret>
template<typename... K>
void dispatch<Tag,Ret>::retract() {
  constexpr std::size_t arity = Tag::traits::vsize;
  constexpr std::size_t btset = Tag::traits::type_bitset;

  functor_t f;

  if(arity == 1) {
    using rpoles = rem_poles_unary<Tag, btset>;
    f = rpoles::template eval<0>( mpl::mplpack<K...>() );
  }
  else {
    std::uintptr_t spec [ arity ];
    rtti_node const* hiers [ arity ];

    using rpoles = rem_poles<Tag, btset>;
    rpoles::template eval<0>( spec, hiers, mpl::mplpack<K...>() );
    f = detail::retract( arity, Tag::invoker_table, spec, hiers );
  }

  typedef typename Tag::traits::trampoline::func_t sig_t;
  typedef std::function<sig_t> func_t;
  delete static_cast<func_t*>(f);
}

}}} // namespace rtti::mmethod::detail

#endif
