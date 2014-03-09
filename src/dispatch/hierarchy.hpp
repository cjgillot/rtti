#ifndef RTTI_MPH_HIERARCHY_HPP
#define RTTI_MPH_HIERARCHY_HPP

#include <cstddef>
#include <map>

#include "early.hpp"

#include "klass.hpp"

class hierarchy_t
{
public:
  typedef std::map<std::size_t, klass_t> dict_t;
  dict_t dict;

public:
  // remove non-pole in hierarchy
  void shrink();

  const klass_t* add(rtti_hierarchy hh);

  template<typename Sequence>
  void order(Sequence& seq);

public:
  std::size_t size() const { return dict.size(); }
};

typedef std::vector<std::vector<const klass_t*>> pole_table_t;

#include "signature.hpp"

template<typename R0, typename R1>
signature_t make_signature(R0 const& r0, R1& r1) {
  signature_t ret ( r1.size() );

  std::transform(
    r0.begin(), r0.end(),
    r1.begin(),

    ret.array_ref().begin(),

    [](rtti_hierarchy a0, hierarchy_t& a1)
    { return a1.add(a0); }
  );

  return ret;
}

#endif
