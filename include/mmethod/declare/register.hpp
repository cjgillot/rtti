//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_REGISTER_HPP
#define RTTI_MMETHOD_DECLARE_REGISTER_HPP

#include "mmethod/declare/helper.hpp"

#include "mmethod/export/table.hpp"

namespace rtti { namespace mmethod {

template<typename Tag, typename Sig>
struct mmethod_register
: detail::register_base<Tag>
, detail::make_declare<Tag, Sig>::type {
private:
  typedef typename detail::make_declare<Tag, Sig>::type decl_maker;

public:
  typedef typename decl_maker::func_t function_type;
  using typename decl_maker::result_type;

  using decl_maker::operator();
  using decl_maker::fetch;
  using decl_maker::insert;
  using decl_maker::generate;
};

}} // namespace rtti::mmethod

#include <boost/config.hpp>

#define MMETHOD_TAG(name) BOOST_JOIN(rtti_mmethod_tags__, name)

#define DECLARE_MMETHOD(name, ret, sig) \
struct MMETHOD_TAG(name)                \
: rtti::mmethod::mmethod_register<      \
  MMETHOD_TAG(name)                     \
, ret sig   > {                         \
  template<typename> struct overload;   \
}; \
static MMETHOD_TAG(name) name /* ; */

#endif
