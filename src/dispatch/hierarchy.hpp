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

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/range/adaptor/transformed.hpp>

/*!\brief Class hierarchy object
 *
 * This class acts as a factory for \c klass objects.
 */
class hierarchy_t
{
public:
  //!\brief Ctor
   hierarchy_t();
  //!\brief Dtor
  ~hierarchy_t();

public:
  //!\brief Compute poles for \c *this
  //!\arg input The static arguments of mmethods
  //!\c *this is filled with the poles
  void compute_poles(std::vector<rtti_hierarchy> const& input);

  //!poles are sorted in reverse subtyping order :
  //!base classes first
  //@{
  typedef std::vector<klass_t* > range_type;
  range_type const& range() const {
      return klasses;
  }
  //@}

  //!\brief Fetch klass from rtti_hierarchy
  klass_t const* fetch(rtti_hierarchy) const;

private:
  void add_pole(rtti_hierarchy, klass_t::bases_type&);
  void effective_bases(rtti_hierarchy, klass_t::bases_type*);
  std::size_t pseudo_closest(rtti_hierarchy, klass_t::bases_type const&, klass_t const**);

private:
  std::vector<klass_t*> klasses;

  typedef boost::unordered_map<rtti_hierarchy, klass_t const*> poles_map_t;
  poles_map_t poles;

  std::size_t current_rank;
};

typedef std::vector<hierarchy_t> pole_table_t;

#include "signature.hpp"

namespace hierarchy_detail {
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

    hierarchy_detail::hierarchy_adder()
  );

  return ret;
}

#endif
