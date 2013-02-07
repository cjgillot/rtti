#ifndef RTTI_MMETHOD_DECLARE_TPP
#define RTTI_MMETHOD_DECLARE_TPP

#include "rtti/mmethod/print.hpp"
#include "rtti/mmethod/regbase.hpp"
#include "rtti/mmethod/mplpack.hpp"
#include "rtti/mmethod/dispatch.hpp"

#include "rtti/mmethod/poles.hpp"

#include "rtti/mmethod/declare.hpp"

#include <type_traits>

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Tag, typename Ret, typename... Args>
struct make_declare_helper<Tag, Ret, Args...>::traits
{
  typedef typename make_declare_helper::trampoline trampoline;

  typedef mpl::mplpack<typename type_traits::remove_all<Args>::type...> raw_args;
  typedef typename raw_args::template transform<tags::is_virtual>::type type_tags;

#ifdef RTTI_MMETHOD_DO_PRINT
  enum {
    type_bitset = boost::mpl::reverse_fold<
      typename type_tags::template to_mpl_seq<boost::mpl::vector>::type,
      boost::mpl::size_t<0>,
      boost::mpl::if_<_2
      , boost::mpl::next<boost::mpl::times<_1, boost::mpl::size_t<2>>>
      , boost::mpl::times<_1, boost::mpl::size_t<2>>
      >
    >::type::value
  };
  typedef mmethod_declare<Tag, vsize, sizeof...(Args), type_bitset> decl;

  // output declaration
  RTTI_PRINT( typename traits::decl );
#endif
};

}}} // namespace rtti::mmethod::detail

#endif
