//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_NULL_HANDLER_HPP
#define MMETHOD_AMBIGUITY_NULL_HANDLER_HPP

#include "mmethod/export/table.hpp"

namespace rtti {
namespace mmethod {
namespace ambiguity {

using detail::ambiguity_handler_t;

struct null_policy {

  BOOST_CONSTEXPR static ambiguity_handler_t
  get_ambiguity_handler() noexcept
  { return NULL; }

};

}}} // namespace rtti::mmethod::ambiguity

#endif
