#ifndef RTTI_MPH_SIGNATURE_HPP
#define RTTI_MPH_SIGNATURE_HPP

#include <vector>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/optional.hpp>

#include "klass.hpp"

class signature_t
{
  typedef std::vector<const klass_t*> sig_type;
  sig_type sig;

public:
  explicit signature_t(std::size_t a)
  : sig(a, NULL) {}

  template<typename Range>
  explicit signature_t(const Range &v)
  : sig(boost::begin(v), boost::end(v)) {}

public:
  sig_type& array_ref() { return sig; }
  sig_type const& array() const { return sig; }

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
