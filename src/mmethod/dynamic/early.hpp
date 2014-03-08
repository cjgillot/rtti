#ifndef EARLY_HPP
#define EARLY_HPP

#include "mmethod/dynamic/poles.hpp"

using namespace rtti;

typedef std::vector<rtti_hierarchy> signature_type;
typedef std::pair<signature_type, invoker_t> binding_type;
struct early_bindings_type {
  std::size_t arity;
  std::vector<binding_type> vector;
};

#define BAD_DISPATCH (rtti::_rtti_bad_dispatch)
using rtti::invoker_t;
using rtti::dmethod::detail::poles_map_type;
using rtti::dmethod::detail::seal_table_type;

#endif
