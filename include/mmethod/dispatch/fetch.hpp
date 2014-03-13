#ifndef RTTI_DMETHOD_DISPATCH_FETCH_HPP
#define RTTI_DMETHOD_DISPATCH_FETCH_HPP

#include "mmethod/dispatch/dispatch.hpp"

#include "mmethod/dynamic/poles.hpp"

#include "mmethod/shared/basic.hpp"
#include "rtti/holder/getter.hpp"

#include "mmethod/hash/fetch_pole.hpp"

#include <functional>
#include <boost/fusion/include/accumulate.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/zip.hpp>
#include <boost/mpl/range_c.hpp>

namespace rtti { namespace dmethod { namespace detail {

namespace {

template<bool OK>
struct get_poles_map {
  template<typename Tag, std::size_t J>
  static poles_map_type& get();
};

template<>
struct get_poles_map<true> {
  template<typename Tag, std::size_t J>
  static poles_map_type& get()
  { return Tag::template poles<J>::array; }
};

template<bool OK>
struct get_node {
  template<typename T>
  static rtti_hierarchy get(T const&);
};

template<>
struct get_node<true> {
  template<typename T>
  static rtti_hierarchy get(T const& t)
  { return ::rtti::get_node(t); }
  template<typename T>
  static rtti_hierarchy get(T& t)
  { return ::rtti::get_node(t); }
};

/// fetch_poles<>::eval(spec,_,args) loops over args and returns the sum of pole-data
/// equivalent pseudo-code :
/// \code
/// i = 0
/// for each arg,
///   if arg is virtual
///     i += arg->pole->data
/// \endcode
template<typename Tag, std::size_t BTS>
struct fetch_poles_once {
  typedef uintptr_t result_type;

  template<typename U>
  uintptr_t operator()(uintptr_t m, U const& x) const {
    typedef typename boost::fusion::result_of::begin<U>::type first_it;
    typedef typename rtti::traits_detail::remove_all<typename boost::fusion::result_of::deref<first_it>::type>::type first;
    enum { J = first::value };

    typedef typename boost::fusion::result_of::next<first_it>::type second_it;
    typedef typename boost::fusion::result_of::deref<second_it>::type second;

    second arg = boost::fusion::deref(boost::fusion::next(boost::fusion::begin(x)));

    enum { ok = (BTS >> J) & 1 };
    if( ok ) {
      poles_map_type& map = get_poles_map<ok>::template get<Tag, J>();

      rtti_hierarchy const h  = get_node<ok>::get(arg);
      m += fetch_pole(map, h);
    }

    return m;
  }
};
template<std::size_t Arity, typename Tag, std::size_t BTS>
struct fetch_poles {
  template<typename Tuple>
  static void eval(uintptr_t& m, Tuple const& args) {
    fetch_poles_once<Tag, BTS> fetcher;
    
    enum { TSize = boost::mpl::size<Tuple>::value };
    m = boost::fusion::accumulate(
      boost::fusion::zip( boost::mpl::range_c<std::size_t, 0, TSize>(), args)
    , uintptr_t(0)
    , fetcher
    );
  }
};

template<std::size_t Arity, typename Tag, std::size_t BTS>
struct fetch_invoker {
  static invoker_t ATTRIBUTE_PURE eval(uintptr_t spec) {
    return Tag::invoker_table[spec / 2];
  }
};
template<typename Tag, std::size_t BTS>
struct fetch_invoker<1, Tag, BTS> {
  static invoker_t ATTRIBUTE_PURE eval(uintptr_t spec) {
    return (invoker_t)spec;
  }
};

} // namespace <>

/// main dispatch function
template<typename Tag, typename Ret>
template<typename Tuple>
invoker_t dispatch<Tag, Ret>::fetch(Tuple const& args) const {
  enum {
    arity = Tag::traits::vsize,
    btset = Tag::traits::type_bitset
  };

  uintptr_t spec = 0;
  fetch_poles<arity, Tag, btset>::eval( spec, args );

  return fetch_invoker<arity, Tag, btset>::eval( spec );
}

}}} // rtti::mmethod::detail

#endif
