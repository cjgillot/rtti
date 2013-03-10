#include "rtti/shared/tags.hpp"
#include "rtti/shared/mplpack.hpp"
#include <type_traits>

namespace rtti {
namespace MMETHOD_NSPACE {
namespace detail {

template<typename Tag, typename Ret, typename... Args>
struct make_declare_helper<Tag, Ret, Args...>::traits
{
  typedef rtti::detail::trampoline<Tag, Ret, typename tags::unwrap<Args>::type...> trampoline;

  typedef mpl::mplpack<typename tags::unwrap<Args>::raw_type...> raw_args;
  typedef mpl::mplpack<typename tags::unwrap<Args>::has_virtual...> type_tags;

  enum {
    vsize = tags::virtual_size<Args...>::value
  ,
    type_bitset = boost::mpl::reverse_fold<
      typename type_tags::template to_mpl_seq<boost::mpl::vector>::type,
      boost::mpl::size_t<0>,
      boost::mpl::if_<mpl_::_2
      , boost::mpl::next<boost::mpl::times<mpl_::_1, boost::mpl::size_t<2>>>
      , boost::mpl::times<mpl_::_1, boost::mpl::size_t<2>>
      >
    >::type::value
  };

#ifdef RTTI_MMETHOD_DO_PRINT
  typedef mmethod_declare<Tag, vsize, sizeof...(Args), type_bitset> decl;

  // output declaration
  RTTI_PRINT( typename traits::decl );
#endif
};

}}} // namespace rtti::mmethod::detail
