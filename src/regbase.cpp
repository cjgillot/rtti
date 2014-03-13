#include "mmethod/shared/basic.hpp"
#include "manip.hpp"

#include <limits>

using namespace rtti::detail;

const rtti::rtti_node
invalid_node::node = {{
//   rtti_type( NULL ),
  0, { NULL }
}};

void rtti::_rtti_bad_dispatch() {
#if __EXCEPTIONS
  throw bad_dispatch();
#else
  std::abort();
#endif
}

rtti::bad_dispatch::bad_dispatch()
: std::runtime_error("Unresolved call for mmethod") {}
rtti::bad_dispatch::~bad_dispatch() BOOST_NOEXCEPT_OR_NOTHROW {}
