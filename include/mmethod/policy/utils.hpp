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

BOOST_TTI_HAS_STATIC_MEMBER_FUNCTION(bad_dispatch)

// {{{ get_ahndl

template<typename Policy>
struct wrap_ahndl {
  static bool ahndl(size_t n, rtti_type* a) {
    return Policy::ambiguity_handler(n, a);
  }
};

// }}}
// {{{ get_bad

template<typename Policy, bool Enable>
struct get_bad {
  template<typename Fp>
  static invoker_t
  get() {
    Fp fp = &Policy::bad_dispatch;
    return reinterpret_cast<invoker_t>(fp);
  }
};

template<typename Policy>
struct get_bad<Policy, false> {
  template<typename Fp>
  static invoker_t
  get() {
    return &Policy::bad_dispatch;
  }
};

// }}}

} // namespace detail

template<typename Policy>
struct get_fpointers {
public:
  static ambiguity_handler_t
  get_ambiguity_handler() {
    return &detail::wrap_ahndl<Policy>::ahndl;
  }

  template<typename Fp>
  static invoker_t
  get_bad_dispatch() {
    typedef detail::has_static_member_function_bad_dispatch<
      Policy
    , typename boost::function_types::result_type<Fp>::type
    , typename boost::function_types::parameter_types<Fp>::type
    > has_bd;

    return detail::get_bad<Policy, has_bd::value>::template get<Fp>();
  }
};

}}} // namespace rtti::mmethod::ambiguity

#endif
