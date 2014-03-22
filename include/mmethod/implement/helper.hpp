//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_IMPLEMENT_HELPER_HPP
#define RTTI_MMETHOD_IMPLEMENT_HELPER_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/trampoline/trampoline.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/function_types/components.hpp>

namespace rtti {
namespace mmethod {
namespace detail {

template<typename Tag, typename Over, typename Ret, typename Args>
struct make_implement_helper {
private:
  typedef typename Tag::traits traits;

  enum { vsize = traits::vsize };

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
