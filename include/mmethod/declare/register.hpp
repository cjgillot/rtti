//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_REGISTER_HPP
#define RTTI_MMETHOD_DECLARE_REGISTER_HPP

#include "mmethod/config.hpp"
#include "mmethod/declare/helper.hpp"

#include "mmethod/policy/default_policy.hpp"

namespace rtti { namespace mmethod {

template<typename Tag, typename Sig, typename Policy>
struct mmethod_register
: detail::make_declare<Tag, Policy, Sig>::type {
private:
  typedef typename detail::make_declare<Tag, Policy, Sig>::type decl_maker;

public:
  typedef typename decl_maker::func_t function_type;
  using typename decl_maker::result_type;

  using decl_maker::operator();

  using decl_maker::call;
  using decl_maker::fetch;

  using decl_maker::fast_call;
  using decl_maker::fast_fetch;

  using decl_maker::insert;
  using decl_maker::generate;
};

}} // namespace rtti::mmethod

#define MMETHOD_TAG(name) BOOST_JOIN(rtti_mmethod_tags__, name)

#define DECLARATION_MMETHOD_POLICY(tag, ret, sig, policy)      \
struct tag                                      \
: rtti::mmethod::mmethod_register<              \
  tag                                           \
, ret sig                                       \
, policy > {                                    \
  template<typename> struct overload;           \
} /* ; */

#define DECLARATION_MMETHOD(tag, ret, sig)      \
DECLARATION_MMETHOD_POLICY(tag, ret, sig, rtti::mmethod::ambiguity::default_policy)

#define DECLARE_MMETHOD_POLICY(name, ret, sig, policy)          \
DECLARATION_MMETHOD_POLICY(MMETHOD_TAG(name), ret, sig, policy);       \
static MMETHOD_TAG(name) name /* ; */

#define DECLARE_MMETHOD(name, ret, sig)                 \
DECLARATION_MMETHOD(MMETHOD_TAG(name), ret, sig);       \
static MMETHOD_TAG(name) name /* ; */

#endif
