//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_GETTER_TRAITS_HPP
#define RTTI_GETTER_TRAITS_HPP

#include "mmethod/config.hpp"
#include "mmethod/rtti/getter/getter.hpp"
#include "mmethod/rtti/mixin/mixin_node.hpp"

namespace rtti {
namespace detail {

template<class M>
struct rtti_getter::traits {
private:
  typedef typename boost::add_pointer<M>::type pointer;

public:
  template<typename D>
  static mixin_node_holder const&
  get_mixin_node(D const& d) {
    return rtti_get_mixin(d);
  }

  BOOST_STATIC_CONSTANT(std::size_t, parents_size = sizeof( rtti_parents_size_1p((pointer)NULL) ) - 1);

  template<typename F>
  static void parents_foreach(F f) {
    rtti_parents_foreach(f, (pointer)NULL);
  }
};

}} // namespace rtti::detail

#endif
