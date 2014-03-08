#ifndef RTTI_DMETHOD_DYNAMIC_HPP
#define RTTI_DMETHOD_DYNAMIC_HPP

#include "mmethod/dynamic/common.hpp"
#include "mmethod/dispatch/dispatch.hpp"

#include "mmethod/declare/declare.hpp"
#include "mmethod/declare/helper.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>

namespace rtti { namespace dmethod {

// namespace {
// 
// template<typename Tag, std::size_t BTS>
// struct init_poles {
//   template<std::size_t J, std::size_t arity>
//   static void eval() {
//     if( BTS & 1 ) {
//       detail::poles_map_type& map = Tag::template poles<J>::array;
// 
//       if( arity == 1 )
//         detail::init_pole_unary(map);
//       else
//         detail::init_pole(map);
//     }
// 
//     using next = init_poles<Tag, BTS/2>;
//     next::template eval<J+1, arity>();
//   }
// };
// template<typename Tag>
// struct init_poles<Tag, 0> {
//   template<std::size_t, std::size_t>
//   static void eval() {}
// };
// 
// } // namespace <>

template<typename Tag>
detail::invoker_table_type register_base<Tag>::invoker_table;

template<typename Tag>
template<std::size_t J>
detail::poles_map_type register_base<Tag>::poles<J>::array;

template<typename Tag>
void register_base<Tag>::do_initialize() {
  constexpr std::size_t arity = Tag::traits::vsize;

  detail::init_table(arity, Tag::invoker_table);
//   constexpr std::size_t btset = Tag::traits::type_bitset;
// 
//   using spoles = init_poles<Tag, btset>;
//   spoles::template eval<0, arity>();
// 
//   if( arity != 1 )
//     detail::init_table(arity, Tag::invoker_table);
}

namespace detail {

namespace {

template<bool OK>
struct get_static_node {
  template<typename T>
  static rtti_hierarchy get();
};

template<>
struct get_static_node<true> {
  template<typename T>
  static rtti_hierarchy get()
  { return ::rtti::static_node<T>(); }
};

template<typename Tag, std::size_t BTS>
struct save_poles_once {
  // workaround mpl::for_each which requires const functor
  mutable rtti_hierarchy* h;

  template<typename U>
  void operator()(U*) const {
    typedef typename boost::mpl::begin<U>::type first_it;
    typedef typename rtti::mpl::remove_all<typename boost::mpl::deref<first_it>::type>::type first;
    enum { J = first::value };

    typedef typename boost::mpl::next<first_it>::type second_it;
    typedef typename rtti::mpl::remove_all<typename boost::mpl::deref<second_it>::type>::type second;

    enum { ok = (BTS >> J) & 1 };
    if( ok ) {
      *h = get_static_node<ok>::template get< second >();
      ++h;
    }
  }
};
template<std::size_t Arity, typename Tag, std::size_t BTS>
struct save_poles {
  template<typename Tuple>
  static void eval(rtti_hierarchy* h, Tuple*) {
    save_poles_once<Tag, BTS> fetcher = { h };

    enum { TSize = boost::mpl::size<Tuple>::value };
    typedef boost::mpl::zip_view<
      boost::mpl::vector<
        boost::mpl::range_c<std::size_t, 0, TSize>
      , Tuple
      >
    > zipped_tuple;

    boost::mpl::for_each<
      zipped_tuple
    , boost::add_pointer<boost::mpl::_>
    >( fetcher );
  }
};

} // namespace <>

template<typename Tag, typename Ret>
template<typename K, typename F>
void dispatch<Tag,Ret>::insert(F&& f) {
  enum {
    arity = Tag::traits::vsize,
    btset = Tag::traits::type_bitset
  };
  rtti_hierarchy hiers [ arity ];

  save_poles<arity, Tag, btset>::eval( hiers, (K*)0 );

  invoker_t inv = reinterpret_cast<invoker_t>(f);

  detail::inse_table(arity, Tag::invoker_table, inv, hiers);
}

// namespace {
// 
// template<typename Tag, std::size_t BTS>
// struct rem_poles {
//   template<std::size_t J, typename K0, typename... Ks>
//   static void eval(std::uintptr_t* m, rtti_hierarchy* h, mpl::mplpack<K0, Ks...>) {
//     if( BTS & 1 ) {
//       *h = ::rtti::static_node< typename mpl::remove_all<K0>::type >();
//       *m = retract_pole( Tag::template poles<J>::array, *h );
//       ++h; ++m;
//     }
// 
//     using next = rem_poles<Tag, BTS/2>;
//     next::template eval<J+1>(m, h, mpl::mplpack<Ks...>());
//   }
// };
// template<typename Tag>
// struct rem_poles<Tag, 0> {
//   template<std::size_t, typename... P>
//   static void eval(P&& ...args) {}
// };
// 
// template<typename Tag, std::size_t BTS>
// struct rem_poles_unary {
//   template<std::size_t J, typename K0, typename... Ks>
//   static functor_t eval(mpl::mplpack<K0, Ks...>) {
//     if( BTS & 1 ) {
//       auto node = ::rtti::static_node< typename mpl::remove_all<K0>::type >();
//       return retract_pole_unary( Tag::template poles<J>::array, node );
//     }
// 
//     using next = rem_poles_unary<Tag, BTS/2>;
//     return next::template eval<J+1>(mpl::mplpack<Ks...>());
//   }
// };
// template<typename Tag>
// struct rem_poles_unary<Tag, 0> {
//   template<std::size_t, typename... P>
//   static functor_t eval(P&& ...args) { return nullptr; }
// };
// 
// } // namespace <>

namespace {

template<std::size_t Arity, typename Tag, std::size_t BTS>
struct seal_poles {
  template<std::size_t J = 0>
  static void eval(poles_map_type** p) {
    enum { ok = BTS & 1 };
    if( ok ) {
      *p = &get_poles_map<ok>::template get<Tag, J>();
      ++p;
    }

    seal_poles<Arity, Tag, (BTS>>1)>::template eval<J+1>(p);
  }
};
template<std::size_t Arity, typename Tag>
struct seal_poles<Arity, Tag, 0> {
  template<std::size_t J = 0>
  static void eval(poles_map_type**) {}
};

} // namespace <>

template<typename Tag, typename Ret>
void dispatch<Tag,Ret>::seal() const {
  enum {
    arity = Tag::traits::vsize,
    btset = Tag::traits::type_bitset
  };

  poles_map_type* poles [ arity ];
  seal_table_type seal_table = { Tag::invoker_table, poles };

  seal_poles<arity, Tag, btset>::eval( poles );

  detail::seal_table(arity, Tag::invoker_table, seal_table);
}

}}} // namespace rtti::mmethod::detail

#endif
