//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_FOREIGN_TYPEINDEX_HPP
#define RTTI_FOREIGN_TYPEINDEX_HPP

#include "mmethod/config.hpp"

#include <typeinfo>

namespace rtti {
namespace foreign_detail {

struct type_index
{
  type_index(std::type_info const& other) BOOST_NOEXCEPT_OR_NOTHROW
  : target(&other) { }

  bool
  operator==(type_index const& other) const BOOST_NOEXCEPT_OR_NOTHROW
  { return *target == *other.target; }

  bool
  operator!=(type_index const& other) const BOOST_NOEXCEPT_OR_NOTHROW
  { return *target != *other.target; }

  bool
  operator<(type_index const& other) const BOOST_NOEXCEPT_OR_NOTHROW
  { return target->before(*other.target); }

  bool
  operator<=(type_index const& other) const BOOST_NOEXCEPT_OR_NOTHROW
  { return !other.target->before(*target); }

  bool
  operator>(type_index const& other) const BOOST_NOEXCEPT_OR_NOTHROW
  { return other.target->before(*target); }

  bool
  operator>=(type_index const& other) const BOOST_NOEXCEPT_OR_NOTHROW
  { return !target->before(*other.target); }

  const char*
  name() const
  { return target->name(); }

private:
  std::type_info const* target;
};

} // namespace foreign_detail
} // namespace rtti

#endif
