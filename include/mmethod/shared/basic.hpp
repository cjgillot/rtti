#ifndef RTTI_MMETHOD_BASIC_HPP
#define RTTI_MMETHOD_BASIC_HPP

#include <stdexcept>
#include <stdint.h>

#include "rtti/interface.hpp"
#include "rtti/detail/attribute.hpp"

#include <boost/static_assert.hpp>

namespace rtti {

typedef void(*invoker_t)();
BOOST_STATIC_ASSERT_MSG( sizeof(invoker_t) <= sizeof(uintptr_t), "Platform not supported" );

struct bad_dispatch
: std::runtime_error {
  bad_dispatch();
  virtual ~bad_dispatch() BOOST_NOEXCEPT_OR_NOTHROW;
};

void ATTRIBUTE_NORETURN _rtti_bad_dispatch();

namespace detail {
struct invalid_node {
  typedef invalid_node type;
  static const rtti_node node;
};
} // namespace detail

} // namespace rtti

#endif
