#ifndef RTTI_DMETHOD_DECLARE_TRAITS_HPP
#define RTTI_DMETHOD_DECLARE_TRAITS_HPP

#include "mmethod/dynamic/poles.hpp"
#include "mmethod/declare/declare.hpp"

#include "mmethod/shared/tags.hpp"

#include <type_traits>
#include <boost/mpl/next.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/reverse_fold.hpp>

namespace rtti {
namespace dmethod {
namespace detail {

template<typename Tag, typename Ret, typename Args>
struct make_declare_helper<Tag, Ret, Args>::traits
{
  typedef typename boost::mpl::transform<Args, tags::unwrap>::type unwrapped_args;
  typedef typename boost::mpl::transform<Args, tags::is_virtual>::type type_tags;

  typedef rtti::detail::trampoline<Tag, Ret, unwrapped_args> trampoline;

  enum {
    vsize = tags::virtual_size<Args>::value
  ,
    type_bitset = boost::mpl::reverse_fold<
      type_tags,
      boost::mpl::size_t<0>,
      boost::mpl::if_<mpl_::_2
      , boost::mpl::next<boost::mpl::times<mpl_::_1, boost::mpl::size_t<2>>>
      , boost::mpl::times<mpl_::_1, boost::mpl::size_t<2>>
      >
    >::type::value
  };
};

}}} // namespace rtti::mmethod::detail

#endif