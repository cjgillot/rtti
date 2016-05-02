//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MPH_HIERARCHY_HPP
#define RTTI_MPH_HIERARCHY_HPP

#include "forward.hpp"
#include "klass.hpp"

#include <boost/unordered_map.hpp>

/*!\brief Class hierarchy object
 *
 * This class acts as a factory for \c klass_t objects.
 * It is meant to be used as a wrapper for these,
 * and is constucted using \c compute_poles method.
 */
class hierarchy_t
{
public:
  //!\brief Ctor
   hierarchy_t();
  //!\brief Dtor
  ~hierarchy_t();

public:
  //! Classes are sorted in reverse subtyping order:
  //! base classes first
  //@{
  typedef std::vector<klass_t* > range_type;
  range_type const& range() const {
      return klasses;
  }
  //@}

  //! Fetch utilities to retrieve \c klass_t objects
  //! from \c rtti_hierarchy references.
  //!
  //! The try_* variants return \c NULL if not found,
  //! the other \c ASSERT.
  //!
  //! The *_from variants upcast until a class is found.
  //@{
  //!\brief Fetch klass from rtti_hierarchy
  klass_t const* fetch(rtti_hierarchy) const;
  //!\brief Like fetch
  //!\return NULL is not found
  klass_t const* try_fetch(rtti_hierarchy) const;

  //!\brief Like try_fetch, but upcast until found
  klass_t const* try_fetch_from(rtti_hierarchy) const;
  //!\brief Like fetch, but upcast until found
  klass_t const* fetch_from(rtti_hierarchy) const;
  //@}

  //! Pseudo-closest algorithm implementation
  //@{
public:
  //!\brief Compute poles for \c *this
  //!\arg input The static arguments of mmethods
  //!\c *this is filled with the poles
  void compute_poles(std::vector<rtti_hierarchy> const& input);

private:
  //!\brief Register new node in hierarchy.
  void add_pole(rtti_hierarchy, klass_t::bases_type&);
  //!\brief Compute that bases that are also poles.
  void effective_bases(rtti_hierarchy, klass_t::bases_type*);
  //!\brief pseudo_closest algorithm to find upwards poles.
  std::size_t pseudo_closest(
    rtti_hierarchy
  , klass_t::bases_type const&
  , klass_t const**
  );
  //@}

private:
  std::vector<klass_t*> klasses;

  typedef boost::unordered_map<rtti_hierarchy, klass_t const*> poles_map_t;
  poles_map_t poles;

  std::size_t current_rank;
};

#endif
