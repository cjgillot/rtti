//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_FOR_EACH_HPP
#define RTTI_MMETHOD_FOR_EACH_HPP

#include "mmethod/config.hpp"

#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Begin, typename End>
struct for_each_impl {
  typedef typename boost::mpl::deref<Begin>::type front_t;
  typedef typename boost::mpl::next<Begin>::type  next_t;

  template<typename F>
  static void apply(F const& f) {
    f(front_t());

    for_each_impl<next_t, End>::apply(f);
  }
};

template<typename B>
struct for_each_impl<B, B> {
  template<typename F>
  static void apply(F const& /*f*/)
  {}
};

template<typename Seq, typename F>
void for_each(F const& f) {
  for_each_impl<
    typename boost::mpl::begin<Seq>::type,
    typename boost::mpl::end<Seq>::type
  >::template apply(f);
}

} // namespace detail
} // namespace mmethod
} // namespace rtti

#endif
