#include "overloads.hpp"

overloads_t::overloads_t(std::size_t a)
: arity(a) {
  values.push_back(std::make_pair(signature_t(arity), invoker_t(0)));
}

void overloads_t::save(invoker_t payload)
{
  values.back().second = payload;
  values.push_back(std::make_pair(signature_t(arity), invoker_t(0)));
}

void overloads_t::finish()
{ values.pop_back(); }

void overloads_t::sort()
{ std::sort(values.begin(), values.end()); }

