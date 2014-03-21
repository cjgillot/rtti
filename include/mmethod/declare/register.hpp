//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_HPP
#define RTTI_MMETHOD_DECLARE_HPP

#include "mmethod/declare/helper.hpp"
#include "mmethod/dynamic/common.hpp"

namespace rtti { namespace dmethod {

template<typename Tag, typename Sig>
struct dmethod_register
: register_base<Tag>
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

#endif
