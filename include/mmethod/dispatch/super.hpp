//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DISPATCH_SUPER_HPP
#define RTTI_MMETHOD_DISPATCH_SUPER_HPP

#include "mmethod/config.hpp"
#include "mmethod/dispatch/common.hpp"

#include "mmethod/detail/mpl.hpp"

#include "mmethod/dispatch/fetch.hpp"

namespace rtti {
namespace mmethod {

namespace dispatch_detail {

/// super_poles<>::eval(spec,_,args) loops over args and returns the sum of pole-data
/// equivalent pseudo-code :
/// \code
/// i = 0
/// for each arg,
///   if arg is virtual
///     i += arg->pole->data
/// \endcode
template<typename Tag, typename Super>
struct super_poles_once {
  uintptr_t& m;

  template<std::size_t J>
  void apply() const {
    poles_map_type& map = get_register<Tag>::template poles<J>();

    typedef typename boost::mpl::at_c<Super, J>::type SuperJ;
    rtti_hierarchy const h  = ::rtti::static_node<SuperJ>();

    m += hash::detail::fetch_pole(map, h);
  }
};
template<std::size_t Arity, typename Tag, std::size_t BTS>
struct super_poles {
  template<typename Super>
  static uintptr_t eval() {
    uintptr_t result = 0;

    super_poles_once<Tag, Super> superer = { result };
    arity_loop<BTS>::apply(superer);

    return result;
  }
};

} // namespace dispatch_detail

/// main dispatch function
template<typename Tag>
template<typename Super>
invoker_t dispatch<Tag>::super() const {
  // verify we have sealed the table
  BOOST_ASSERT(detail::get_register<Tag>::early() == NULL);
  typedef detail::access::traits<Tag> traits_type;

  enum {
    arity = traits_type::virtual_size,
    btset = traits_type::tags_bitset
  };

  uintptr_t spec = dispatch_detail::super_poles  <arity, Tag, btset>::template eval<Super>();
  invoker_t ret  = dispatch_detail::fetch_invoker<arity, Tag, btset>::eval( spec );
  BOOST_ASSERT(ret);

  return ret;
}

} // namespace mmethod
} // namespace rtti

#endif
