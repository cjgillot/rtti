//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_TRAITS_HPP
#define RTTI_MMETHOD_DECLARE_TRAITS_HPP

#include "mmethod/config.hpp"
#include "mmethod/traits/tags.hpp"
#include "mmethod/detail/mpl.hpp"

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Ret, typename Policy, typename Args>
struct make_declare_traits
{
  typedef Ret    return_type;
  typedef Args   argument_types;
  typedef Policy policy_type;

  typedef typename boost::mpl::transform<Args, tags::unwrap>::type unwrapped_argument_types;
  typedef typename boost::mpl::transform<Args, tags::is_virtual>::type argument_tags;

  BOOST_STATIC_CONSTANT(std::size_t, virtual_size = tags::virtual_size<Args>::value);
  BOOST_STATIC_CONSTANT(std::size_t, tags_bitset = (boost::mpl::reverse_fold<
      argument_tags,
      boost::mpl::size_t<0>,
      boost::mpl::if_<mpl_::_2
      , boost::mpl::next<boost::mpl::times<mpl_::_1, boost::mpl::size_t<2> > >
      , boost::mpl::times<mpl_::_1, boost::mpl::size_t<2> >
      >
    >::type::value
  ));

  BOOST_STATIC_ASSERT_MSG( (virtual_size > 0), "At least one virtual parameter must be provided." );
};

} // namespace detail
} // namespace mmethod
} // namespace rtti

#endif
