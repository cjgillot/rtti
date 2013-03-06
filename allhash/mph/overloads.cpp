#include "overloads.hpp"

overloads_t::overloads_t(std::size_t a)
: arity(a) { save(); }

void overloads_t::save()
{ values.emplace_back(arity); }

void overloads_t::finish()
{ values.pop_back(); }

void overloads_t::sort()
{ std::sort(values.begin(), values.end(), signature_t::total_order()); }

