#ifndef RTTI_MPH_SIGNATURE_HPP
#define RTTI_MPH_SIGNATURE_HPP

#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/optional.hpp>

#include "klass.hpp"

class signature_t
{
  std::vector<const klass_t*> sig;

public:
  explicit signature_t(std::size_t a)
  : sig(a, nullptr) {}

  template<typename Range>
  explicit signature_t(const Range &v)
  : sig(boost::begin(v), boost::end(v)) {}

  signature_t(const signature_t&) = default;
  signature_t& operator=(const signature_t&) = default;

  signature_t(signature_t&&) = default;
  signature_t& operator=(signature_t&&) = default;

public:
  std::vector<const klass_t*>& array_ref() { return sig; }
  std::vector<const klass_t*> const& array() const { return sig; }

public:
  // total[extended] subtyping order
  struct total_order
  {
    bool operator()(const signature_t& a, const signature_t& b) const;
  };
  // partial subtyping order
  struct subtypes
  {
    bool operator()(const signature_t& a, const signature_t& b) const;
  };

  friend bool operator==(const signature_t& a, const signature_t& b)
  { return a.sig == b.sig; }
};

#endif
