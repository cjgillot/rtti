//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_FOREIGN_HPP
#define BOOST_MMETHOD_FOREIGN_HPP

#include "boost/mmethod/config.hpp"

#include "boost/mmethod/rtti/mixin/mixin_node.hpp"
#include "boost/mmethod/rtti/holder/holder.hpp"

#include "boost/mmethod/rtti/implement_traits.hpp"
#include "boost/mmethod/rtti/foreign/type_index.hpp"

#include <map>

namespace boost {
namespace mmethod {
namespace foreign_detail {

using mixin_detail::mixin_node_holder;

typedef std::map<type_index, mixin_node_holder> map_type;

template<typename Klass>
struct foreign_node_holder_initalizer {
  foreign_node_holder_initalizer() {
    map_type& map = rtti_get_foreign_map((Klass*)NULL);

    typedef typename holder_detail::get_holder<Klass>::type holder_t;

    type_index idx ( typeid(Klass) );
    mixin_node_holder hld = { holder_t::get_node() };
    map.insert(map_type::value_type(idx, hld));
  }
  void touch() const {}
};

template<typename Klass>
mixin_node_holder const&
fetch_mixin_node_holder(Klass const& d) {
  map_type& map = rtti_get_foreign_map((Klass*)NULL);
  map_type::iterator it = map.find( typeid(d) );
  BOOST_ASSERT(it != map.end());
  return it->second;
}

#define BOOST_MMETHOD_FOREIGN_IMPLEMENT(Klass, Supers)                  \
extern boost::mmethod::detail::                                         \
  implement_traits<Klass, Supers>::arity_type                           \
rtti_parents_size_1p(Klass const volatile*);                            \
template<typename F>                                                    \
inline void rtti_parents_foreach(F f, Klass const volatile*) {          \
  boost::mpl::for_each<                                                 \
    boost::mmethod::detail::implement_traits<Klass, Supers>::parents,   \
    boost::add_pointer<boost::mpl::_>                                   \
  >(f);                                                                 \
}                                                                       \
static boost::mmethod::foreign_detail::                                 \
  foreign_node_holder_initalizer<Klass>                                 \
  BOOST_JOIN(__rtti_mmethod_foreign_initializer_, __LINE__);            \
boost::mmethod::mixin_detail::mixin_node_holder const&                  \
rtti_get_mixin(Klass const& d) {                                        \
  BOOST_JOIN(__rtti_mmethod_foreign_initializer_, __LINE__).touch();    \
  return boost::mmethod::foreign_detail::                               \
    fetch_mixin_node_holder<Klass>(d);                                  \
}

#define BOOST_MMETHOD_FOREIGN_DECLARE(base)                     \
inline boost::mmethod::foreign_detail::map_type&                \
rtti_get_foreign_map(base const*) {                             \
  static boost::mmethod::foreign_detail::map_type map;          \
  return map;                                                   \
}                                                               \
BOOST_MMETHOD_FOREIGN_IMPLEMENT(base, boost::mpl::vector<>)

}}} // namespace boost::mmethod::foreign_detail

#endif
