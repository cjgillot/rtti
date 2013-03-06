#ifndef RTTI_MMETHOD_MPLPACK_HPP
#define RTTI_MMETHOD_MPLPACK_HPP

#include <boost/mpl/placeholders.hpp>

#include <boost/mpl/bind.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/placeholders.hpp>

namespace rtti {

using namespace boost::mpl::placeholders;

namespace mpl {

template<std::size_t... Elts>
struct mplpack_c {};

template<typename... Elts>
struct mplpack {
  template<template<typename...> class Seq>
  struct to_mpl_seq {
    typedef Seq<Elts...> type;
  };

  template<typename MF>
  struct transform {
    typedef mplpack<typename boost::mpl::lambda<MF>::type::template apply<Elts>::type...> type;
  };

  template<typename Seq, typename MF>
  struct transform2
  : boost::mpl::transform<
      typename to_mpl_seq<boost::mpl::vector>::type
    , Seq, MF>
  {};

  template<std::size_t I>
  struct get;

  struct as_pack_c {
    typedef mplpack_c<0, std::size_t(Elts::value)...> type;
  };
};

template<typename... Elts1>
template<typename MF, typename... Elts2>
struct mplpack<Elts1...>::transform2<mplpack<Elts2...>, MF> {
  typedef mplpack<typename boost::mpl::lambda<MF>::type::template apply<Elts1, Elts2>::type...> type;
};

template<std::size_t I, typename... Args>
struct get_nth;
template<std::size_t I, typename Arg0, typename... Args>
struct get_nth<I,Arg0,Args...>
: get_nth<I-1, Args...> {};
template<typename Arg0, typename... Args>
struct get_nth<0, Arg0, Args...> {
  typedef Arg0 type;
};

template<typename... Elts>
template<std::size_t I>
struct mplpack<Elts...>::get
: get_nth<I, Elts...> {};

template<typename T>
struct protect {
  typedef T type;
};

}} // namespace rtti::mpl

#endif
