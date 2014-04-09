//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EARLY_HPP
#define EARLY_HPP

#include "boost/mmethod/export/table.hpp"
#include "boost/mmethod/export/exception.hpp"

#include <vector>

using namespace boost::mmethod;
using boost::mmethod::detail::invoker_t;

typedef std::vector<rtti_hierarchy> signature_type;
typedef std::pair<signature_type, invoker_t> binding_type;
struct early_bindings_type {
  std::size_t arity;
  std::vector<binding_type> vector;
};

using boost::mmethod::detail::poles_map_type;
using boost::mmethod::detail::seal_table_type;
using boost::mmethod::detail::ambiguity_handler_t;

namespace boost_mmethod_dispatch {
  // dispatch/mmethod.cpp
  extern void process_declaration(early_bindings_type const&, seal_table_type&);
}

#endif
