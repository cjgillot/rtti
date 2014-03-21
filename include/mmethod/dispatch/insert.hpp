//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DYNAMIC_HPP
#define RTTI_MMETHOD_DYNAMIC_HPP

#include "mmethod/dispatch/forward.hpp"
#include "mmethod/dispatch/fetch.hpp"

#include "mmethod/declare/traits.hpp"

#include "mmethod/dynamic/poles.hpp"

#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>

namespace rtti { namespace dmethod { namespace detail {

template<typename Tag>
invoker_table_type register_base<Tag>::invoker_table;

template<typename Tag>
template<std::size_t J>
poles_map_type register_base<Tag>::poles<J>::array;

template<typename Tag>
void register_base<Tag>::do_initialize() {
  enum { arity = Tag::traits::vsize };

  detail::init_table(arity, Tag::invoker_table);
}

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
    typedef typename boost::mpl::at_c<U, 0>::type first_raw;
    typedef typename rtti::traits_detail::remove_all<first_raw>::type first;
    enum { J = first::value };

    typedef typename boost::mpl::at_c<U, 1>::type second_raw;
    typedef typename rtti::traits_detail::remove_all<second_raw>::type second;

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
void dispatch<Tag,Ret>::insert(F const& f) {
  enum {
    arity = Tag::traits::vsize,
    btset = Tag::traits::type_bitset
  };
  rtti_hierarchy hiers [ arity ];

  save_poles<arity, Tag, btset>::eval( hiers, (K*)0 );

  invoker_t inv = reinterpret_cast<invoker_t>(f);

  detail::inse_table(arity, Tag::invoker_table, inv, hiers);
}

namespace {

template<std::size_t Arity, typename Tag, std::size_t BTS>
struct seal_poles {
  template<std::size_t J>
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
  template<std::size_t J>
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

  seal_poles<arity, Tag, btset>::template eval<0>( poles );

  detail::seal_table(arity, Tag::invoker_table, seal_table);
}

}}} // namespace rtti::mmethod::detail

#endif
