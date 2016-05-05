//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_UTILS_HPP
#define MMETHOD_AMBIGUITY_UTILS_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"
#include "mmethod/policy/forward.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/function_types/components.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace rtti {
namespace mmethod {
namespace ambiguity {

template<typename Policy>
struct policy_traits;

namespace detail {

template<class T, typename Fp>
struct has_static_member_function_bad_dispatch {
private:
  template<Fp> struct helper;

  template<typename U> static boost::type_traits::yes_type check(helper<&U::bad_dispatch>*);
  template<typename U> static boost::type_traits::no_type  check(...);

public:
  enum {
    value = sizeof(check<T>(NULL)) == sizeof(boost::type_traits::yes_type)
  };
};

// {{{ get_ahndl

template<typename Policy>
struct wrap_ahndl {
  static action_t ahndl(size_t n, rtti_hierarchy* a) {
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
struct policy_traits {
public:
  static ambiguity_handler_t
  get_ambiguity_handler() {
    return &detail::wrap_ahndl<Policy>::ahndl;
  }

  template<typename Fp>
  static invoker_t
  get_bad_dispatch() {
    typedef detail::has_static_member_function_bad_dispatch<
      Policy, Fp
    > has_bd;

    return detail::get_bad<Policy, has_bd::value>::template get<Fp>();
  }
};

} // namespace ambiguity
} // namespace mmethod
} // namespace rtti

#endif
