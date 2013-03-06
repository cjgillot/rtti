#ifndef RTTI_MMETHOD_BASIC_HPP
#define RTTI_MMETHOD_BASIC_HPP

#include <stdexcept>

#include "rtti/interface.hpp"
#include "rtti/shared/mpl.hpp"
#include "util/attribute.hpp"

namespace rtti {

typedef void(*invoker_t)();
static_assert( sizeof(invoker_t) <= sizeof(std::uintptr_t), "Platform not supported" );

struct bad_dispatch
: std::runtime_error {
  bad_dispatch();
  virtual ~bad_dispatch() noexcept;
};

void ATTRIBUTE_NORETURN _rtti_bad_dispatch();

namespace detail {
struct invalid_node {
  typedef invalid_node type;
  static const rtti_node node;
};
} // namespace detail

} // namespace rtti

#include "rtti/shared/trampoline.hpp"

#endif
