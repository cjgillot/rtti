//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_IMPLEMENT_HELPER_HPP
#define RTTI_MMETHOD_IMPLEMENT_HELPER_HPP

#include "mmethod/config.hpp"
#include "mmethod/detail/access.hpp"
#include "mmethod/declare/trampoline.hpp"

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Tag, typename Over, typename Ret, typename Args>
struct make_implement_helper {
private:
  typedef access::trampoline<Tag> trampoline;
  typedef access::traits<Tag> traits;

  enum { vsize = traits::vsize };

protected:
  typedef typename trampoline::template apply<Over, Ret, Args> callback;

  typedef make_implement_helper impl_maker;

  make_implement_helper() BOOST_NOEXCEPT_OR_NOTHROW {
    Tag().template insert<Args>( &callback::call );
  }
};

template<typename Tag, typename Over, typename Sig>
struct make_implement {
private:
  typedef typename boost::function_types::result_type<Sig>::type result;
  typedef typename boost::function_types::parameter_types<Sig>::type args;

public:
  typedef make_implement_helper<Tag, Over, result, args> type;
};

}}} // namespace rtti::mmethod::detail

#endif
