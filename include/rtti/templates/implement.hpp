#include "rtti/shared/basic.hpp"
#include "rtti/shared/mplpack.hpp"
#include "rtti/shared/trampoline.hpp"

#include <boost/mpl/bind.hpp>

namespace rtti {
namespace MMETHOD_NSPACE {
namespace detail {

template<typename Tag, typename Over, typename Ret, typename... Args>
struct make_implement_helper {
private:
  typedef mpl::mplpack<Args...> args;
  typedef typename Tag::traits traits;

  enum { vsize = traits::vsize };

protected:
  template<std::size_t I>
  struct get_node {
  private:
    using arg_t = typename args::template get<I>::type;
    using klass_t = typename rtti::mpl::remove_all<arg_t>::type;
    using virtual_t = typename Tag::traits::type_tags::template get<I>::type;
    using dispatch_t = typename boost::mpl::if_<
      virtual_t
    , ::rtti::detail::get_holder<klass_t>
    , ::rtti::detail::invalid_node
    >::type::type;

  public:
    constexpr static rtti_node const* value = &dispatch_t::node;
  };

#ifdef RTTI_MMETHOD_DO_PRINT
private:
  typedef typename args::template transform2<
    typename traits::type_tags
  , boost::mpl::if_<
      _2
    , boost::mpl::bind<tags::make_hierarchy, _1>
    , rtti::hash::path_node<0, rtti::hash::path_end>
    >
  >::type type_hiers;

public:
  typedef mmethod_implement<Tag, vsize, sizeof...(Args), type_hiers> decl;
#endif

protected:
  typedef typename traits::trampoline::template apply<Over, Ret, Args...> trampoline;

  typedef make_implement_helper impl_maker;

  make_implement_helper() noexcept {
    MMETHOD_IMPLEMENT_CTOR_CODE();
  }
};

template<typename Tag, typename Over, typename Ret, typename... Args>
make_implement_helper<Tag, Over, Ret, Args...> make_implement_help(Ret(*f)(Args...));

template<typename Tag, typename Over, typename Sig>
using make_implement = decltype( make_implement_help<Tag, Over>( std::declval<Sig*>() ) );

template<typename Tag, typename Ret, typename... Args>
struct make_hashes_helper {
  static_assert( sizeof...(Args) == Tag::traits::type_tags::length, "Signature arity mismatch" );

  typedef mpl::mplpack<typename rtti::pointer_traits<Args>::raw_type...> args;
  typedef typename args::template transform2<
    typename Tag::traits::type_tags
  , boost::mpl::if_<
      mpl_::_2
    , boost::mpl::bind<hash::detail::hash, mpl_::_1>
    , boost::mpl::size_t<0>
    >
  >::type hashes_t;
  typedef typename hashes_t::as_pack_c::type hashes;
};

template<typename Tag, typename Ret, typename... Args>
make_hashes_helper<Tag, Ret, Args...> make_hashes_help(Ret(*f)(Args...));

template<typename Tag, typename Sig>
using make_hashes = decltype( make_hashes_help<Tag>( std::declval<Sig*>() ) );

}}} // namespace rtti::mmethod::detail
