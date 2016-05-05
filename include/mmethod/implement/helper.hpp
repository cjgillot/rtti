//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_IMPLEMENT_HELPER_HPP
#define RTTI_MMETHOD_IMPLEMENT_HELPER_HPP

#include "mmethod/config.hpp"
#include "mmethod/detail/access.hpp"
#include "mmethod/declare/trampoline.hpp"

#include "mmethod/policy/utils.hpp"
#include "mmethod/detail/for_each.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/function_types/components.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Tag, typename Over, typename Ret, typename Args>
struct make_implement_helper {
private:
  typedef access::trampoline<Tag> trampoline;
  typedef access::traits<Tag> traits;
  typedef typename trampoline::template apply<Over, Ret, Args> callback;

  struct tag_insert {
    template<typename T>
    void operator()(T*) const {
      typedef typename boost::mpl::front<T>::type args_type;
      typedef typename boost::mpl::back<T>::type  call_type;

      Tag().template insert<args_type>(&call_type::call);
    }
  };

protected:
  typedef make_implement_helper impl_maker;

  make_implement_helper() BOOST_NOEXCEPT_OR_NOTHROW {
    typedef typename traits::policy policy_type;
    typedef ambiguity::policy_traits<policy_type> policy_traits;

    typedef typename policy_traits::template get_duplicates<
      Tag, Args, callback
    >::type duplicates_type;
    BOOST_STATIC_ASSERT(! boost::mpl::empty<duplicates_type>::value);

    detail::for_each<duplicates_type>(tag_insert());
  }
};

template<typename Tag, typename Over, typename Sig>
struct make_implement {
private:
  typedef typename boost::function_types::components<Sig>::type components;
  typedef typename boost::mpl::front<components>::type          result;
  typedef typename boost::mpl::pop_front<components>::type      args;

public:
  typedef make_implement_helper<Tag, Over, result, args> type;
};

} // namespace detail
} // namespace mmethod
} // namespace rtti

#endif
