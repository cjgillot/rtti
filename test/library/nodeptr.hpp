//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/export/table.hpp"

using namespace rtti;
using namespace rtti::detail;

struct nodeptr {
  rtti_node* node;

  explicit
  nodeptr(size_t arity) {
    node = static_cast<rtti_node*>(::operator new(
        sizeof(holder_::rtti_node_var<0>)
      + sizeof(rtti_hierarchy) * arity
    ));

    node->self.__arity = arity;
  }

  ~nodeptr() {
    delete node;
  }
};

// {{{ check size computation
namespace {

template<size_t N>
struct check_size
{
  enum {
    thsize = sizeof(size_t) + N*sizeof(rtti_hierarchy),
    resize = sizeof(holder_::rtti_node_var<N>),

    value =
        check_size<N-1>::value &&
        resize == thsize
  };
};
template<>
struct check_size<0>
{
  enum {
    thsize = sizeof(size_t),
    resize = sizeof(holder_::rtti_node_var<0>),

    value =
        resize == thsize
  };
};

BOOST_STATIC_ASSERT(check_size<5>::value);

} // namespace
// }}}

