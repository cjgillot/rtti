//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_ACCESS_HPP
#define RTTI_MMETHOD_DECLARE_ACCESS_HPP

#include "mmethod/config.hpp"

namespace rtti { namespace mmethod { namespace detail {

struct access {

  template<typename Tag>
  struct traits
  : Tag::traits_type
  {};

  template<typename Tag>
  struct trampoline
  : Tag::trampoline_type
  {};

};

}}} // namespace rtti::mmethod::detail

#endif
