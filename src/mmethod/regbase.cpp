#include "rtti/mmethod/regbase.hpp"
#include "rtti/mmethod/basic.hpp"
#include "rtti/mmethod/common.hpp"

#include <limits>

using namespace rtti::mmethod::detail;

const rtti::rtti_node invalid_node::node = {
  rtti_type( std::numeric_limits<std::uintptr_t>::max() ),
  nullptr
};

void rtti::mmethod::_rtti_bad_dispatch() {
#if __EXCEPTIONS__
  throw bad_dispatch();
#else
  std::abort();
#endif
}

rtti::mmethod::bad_dispatch::bad_dispatch()
: std::runtime_error("Unresolved call for mmethod") {}
rtti::mmethod::bad_dispatch::~bad_dispatch() noexcept {}
