#include "rtti/mmethod/regbase.hpp"
#include "rtti/mmethod/common.hpp"

#include "rtti/shared/basic.hpp"

#include <limits>

using namespace rtti::detail;

const rtti::rtti_node
invalid_node::node = {
  rtti_type( std::numeric_limits<std::uintptr_t>::max() ),
  nullptr
};

void rtti::_rtti_bad_dispatch() {
#if __EXCEPTIONS
  throw bad_dispatch();
#else
  std::abort();
#endif
}

rtti::bad_dispatch::bad_dispatch()
: std::runtime_error("Unresolved call for mmethod") {}
rtti::bad_dispatch::~bad_dispatch() noexcept {}
