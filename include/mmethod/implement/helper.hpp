#ifndef RTTI_DMETHOD_IMPLEMENT_HELPER_HPP
#define RTTI_DMETHOD_IMPLEMENT_HELPER_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/trampoline/trampoline.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/function_types/components.hpp>

namespace rtti {
namespace dmethod {
namespace detail {

template<typename Tag, typename Over, typename Ret, typename Args>
struct make_implement_helper {
private:
  typedef typename Tag::traits traits;

  enum { vsize = traits::vsize };

// protected:
//   template<std::size_t I>
//   struct get_node {
//   private:
//     using arg_t = typename Args::template get<I>::type;
//     using klass_t = typename rtti::mpl::remove_all<arg_t>::type;
//     using virtual_t = typename Tag::traits::type_tags::template get<I>::type;
//     using dispatch_t = typename boost::mpl::if_<
//       virtual_t
//     , ::rtti::detail::get_holder<klass_t>
//     , ::rtti::detail::invalid_node
//     >::type::type;
// 
//   public:
//     constexpr static rtti_node const* value = &dispatch_t::node;
//   };

protected:
  typedef typename traits::trampoline::template apply<Over, Ret, Args> trampoline;

  typedef make_implement_helper impl_maker;

  make_implement_helper() BOOST_NOEXCEPT_OR_NOTHROW {
    Tag().template insert<Args>( &impl_maker::trampoline::call );
  }
};

template<typename Tag, typename Over, typename Sig>
struct make_implement {
private:
  typedef boost::function_types::components<Sig> components;
  typedef typename boost::mpl::front<components>::type result;
  typedef typename boost::mpl::pop_front<components>::type args;
  
public:
  typedef make_implement_helper<Tag, Over, result, args> type;
};

}}} // namespace rtti::mmethod::detail

#endif
