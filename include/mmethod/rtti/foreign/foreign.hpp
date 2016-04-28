//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_FOREIGN_HPP
#define RTTI_FOREIGN_HPP

#include "mmethod/config.hpp"

#include "mmethod/rtti/mixin/mixin_node.hpp"
#include "mmethod/rtti/holder/holder.hpp"

#include "mmethod/rtti/implement_traits.hpp"
#include "mmethod/rtti/foreign/type_index.hpp"

#include <map>

namespace rtti {
namespace foreign_detail {

using rtti::detail::mixin_node_holder;

typedef std::map<type_index, mixin_node_holder> map_type;

template<typename Klass>
struct foreign_node_holder_initalizer {
  foreign_node_holder_initalizer() {
    map_type& map = rtti_get_foreign_map((Klass*)NULL);

    typedef typename detail::get_holder<Klass>::type holder_t;

    type_index idx ( typeid(Klass) );
    mixin_node_holder hld = { holder_t::get_node() };
    map.insert(map_type::value_type(idx, hld));
  }
  void touch() const {}
};

template<typename Klass>
detail::mixin_node_holder const&
fetch_mixin_node_holder(Klass const& d) {
  map_type& map = rtti_get_foreign_map((Klass*)NULL);
  map_type::iterator it = map.find( typeid(d) );
  BOOST_ASSERT(it != map.end());
  return it->second;
}

#define MMETHOD_FOREIGN_IMPLEMENT(Klass, Supers)                        \
extern rtti::detail::implement_traits<Klass, Supers>::arity_type        \
rtti_parents_size_1p(Klass const volatile*) MMETHOD_ATTRIBUTE_UNUSED;   \
template<typename F>                                                    \
inline void rtti_parents_foreach(F f, Klass const volatile*) {          \
  boost::mpl::for_each<                                                 \
    rtti::detail::implement_traits<Klass, Supers>::parents,             \
    boost::add_pointer<boost::mpl::_>                                   \
  >(f);                                                                 \
}                                                                       \
static rtti::foreign_detail::foreign_node_holder_initalizer<Klass>      \
  BOOST_JOIN(__rtti_mmethod_foreign_initializer_, __LINE__);            \
rtti::detail::mixin_node_holder const&                                  \
rtti_get_mixin(Klass const& d) {                                        \
  BOOST_JOIN(__rtti_mmethod_foreign_initializer_, __LINE__).touch();    \
  return rtti::foreign_detail::fetch_mixin_node_holder<Klass>(d);       \
}

#define MMETHOD_FOREIGN_DECLARE(base)           \
inline rtti::foreign_detail::map_type&          \
rtti_get_foreign_map(base const*) {             \
  static rtti::foreign_detail::map_type map;    \
  return map;                                   \
}                                               \
MMETHOD_FOREIGN_IMPLEMENT(base, boost::mpl::vector<>)

} // namespace foreign_detail
} // namespace rtti

#endif
