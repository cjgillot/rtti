//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MPH_HIERARCHY_HPP
#define RTTI_MPH_HIERARCHY_HPP

#include <cstddef>
#include <map>

#include "early.hpp"

#include "klass.hpp"

#include <boost/unordered_map.hpp>

class hierarchy_t
{
public:
  std::vector<klass_t* > klasses;

  typedef boost::unordered_map<rtti_type, klass_t*> dict_t;
  dict_t dict;

  std::size_t current_rank;
  
public:
   hierarchy_t();
  ~hierarchy_t();

public:
  const klass_t* add(rtti_hierarchy hh);

  void order(std::vector<klass_t const*>& seq);

public:
  std::size_t size() const { return klasses.size(); }
  
private:
  klass_t* do_add(rtti_hierarchy hh);

  void shrink(std::vector<klass_t const*>& seq);
  void pole_init(klass_t*);
  std::size_t pseudo_closest(klass_t const* k, klass_t const* &pole);
};

typedef std::vector<std::vector<const klass_t*> > pole_table_t;

#include "signature.hpp"

namespace {
  struct hierarchy_adder {
    klass_t const* operator()(rtti_hierarchy a0, hierarchy_t& a1) const
    { return a1.add(a0); }
  };
} // namespace <>

template<typename R0, typename R1>
signature_t make_signature(R0 const& r0, R1& r1) {
  signature_t ret ( r1.size() );

  std::transform(
    r0.begin(), r0.end(),
    r1.begin(),

    ret.array_ref().begin(),

    hierarchy_adder()
  );

  return ret;
}

#endif
