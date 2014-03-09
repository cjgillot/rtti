#ifndef RTTI_MPH_OVERLOADS_HPP
#define RTTI_MPH_OVERLOADS_HPP

#include <vector>

#include <algorithm>
#include <functional>

#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include "hierarchy.hpp"
#include "signature.hpp"

#include "early.hpp"

class overloads_t;

typedef std::pair<signature_t, invoker_t> signature_binding_type;

inline bool operator<(signature_binding_type const& a, signature_binding_type const& b)
{ return signature_t::total_order()(a.first, b.first); }

class overloads_t
{
private:
  std::size_t arity;
  std::vector<signature_binding_type> values;

public:
  explicit
  overloads_t(std::size_t arity);

  void save(invoker_t payload);
  void finish();

  void sort();

public:
  klass_t const*& get_back(std::size_t i)
  { return values.back().first.sig[i]; }

  std::vector<signature_binding_type> const& array() const { return values; }
};

typedef std::map<signature_t, boost::optional<signature_binding_type>, signature_t::total_order> dispatch_t;

namespace std {
  template<>
  struct hash<signature_t>
  {
    std::size_t operator()(const signature_t& s) const
    {
      return boost::hash_range(s.array().begin(), s.array().end());
    };
  };
}

#endif
