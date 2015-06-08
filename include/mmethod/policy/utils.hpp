//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_UTILS_HPP
#define MMETHOD_AMBIGUITY_UTILS_HPP

#include "mmethod/export/table.hpp"
#include "mmethod/detail/iterate.hpp"

#include <boost/tti/has_static_member_function.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

namespace rtti {
namespace mmethod {
namespace ambiguity {

using detail::ambiguity_handler_t;

template<typename Policy>
struct get_fpointers;

namespace detail {

BOOST_TTI_HAS_STATIC_MEMBER_FUNCTION(ambiguity_handler)
BOOST_TTI_HAS_STATIC_MEMBER_FUNCTION(bad_dispatch)

template<typename Policy, bool Enable>
struct get_ahndl {
  static ambiguity_handler_t
  get() {
    return &Policy::ambiguity_handler;
  }
};

template<typename Policy>
struct get_ahndl<Policy, false> {
  static ambiguity_handler_t
  get() {
    return NULL;
  }
};

template<typename Policy, bool Enable>
struct get_bad {
  template<typename Fp>
  static Fp
  get() {
    return &Policy::bad_dispatch;
  }
};

template<typename Policy>
struct get_bad<Policy, false> {
  template<typename Fp>
  static Fp
  get() {
    typedef typename boost::remove_pointer<Fp>::type Sig;
    typedef typename Policy::template rebind<Sig>::other rebound_policy;
    return &rebound_policy::bad_dispatch;
  }
};

} // namespace detail

template<typename Policy>
struct get_fpointers {
public:
  static ambiguity_handler_t
  get_ambiguity_handler() {
    typedef detail::has_static_member_function_ambiguity_handler<
      Policy
    , void, boost::mpl::vector<size_t, rtti_type const*>
    > has_ahndl;

    return detail::get_ahndl<Policy, has_ahndl::value>::get();
  }

  template<typename Fp>
  static invoker_t
  get_bad_dispatch() {
    typedef detail::has_static_member_function_bad_dispatch<
      Policy
    , typename boost::function_types::result_type<Fp>::type
    , typename boost::function_types::parameter_types<Fp>::type
    > has_bd;

    Fp fp = detail::get_bad<Policy, has_bd::value>::template get<Fp>();
    return reinterpret_cast<invoker_t>(fp);
  }
};

}}} // namespace rtti::mmethod::ambiguity

#endif
