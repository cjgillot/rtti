//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

  static signature_t unary(klass_t const* k) {
    klass_t const* range [ 1 ] = { k };
    return signature_t( range );
  }

public:
  sig_type& array_ref() { return sig; }
  sig_type const& array() const { return sig; }

public:
  // total[extended] subtyping order
  struct total_order
  {
    bool operator()(const signature_t& a, const signature_t& b) const;
  };
  // worse match order
  struct worse_match
  {
    bool operator()(const signature_t& a, const signature_t& b) const;
  };

  friend bool operator==(const signature_t& a, const signature_t& b)
  { return a.sig == b.sig; }

  friend bool operator!=(const signature_t& a, const signature_t& b)
  { return a.sig != b.sig; }
};

#include "hierarchy.hpp"

namespace detail {

struct hierarchy_adder {
  klass_t const* operator()(rtti_hierarchy a0, hierarchy_t& a1) const
  {
    klass_t const* ret = a1.fetch(a0);
    BOOST_ASSERT(ret);
    return ret;
  }
};

} // namespace detail

template<typename R0>
signature_t make_signature(R0 const& r0, pole_table_t& r1) {
  signature_t ret ( r1.size() );

  std::transform(
    r0.begin(), r0.end(),
    r1.begin(),

    ret.array_ref().begin(),

    detail::hierarchy_adder()
  );

  return ret;
}

#endif
