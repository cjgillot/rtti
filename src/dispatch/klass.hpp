//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MPH_KLASS_HPP
#define RTTI_MPH_KLASS_HPP

#include <cstddef>

#include <boost/noncopyable.hpp>
#include <boost/dynamic_bitset.hpp>

#include "early.hpp"

class hierarchy_t;
class signature_t;
struct klass_t
: private boost::noncopyable
{
public:
  typedef std::vector<klass_t const*> bases_type;

private:
  friend class hierarchy_t;

  rtti_type const id;
  bases_type bases;

  klass_t const* pole;

  /// \invariant : subtype[o.rank] = 1 - iff [this] derives from [o]
  /// \invariant : subtype[rank] = 1 - consequence
  /// \invariant : subtype[0] is pole flag then : rank == 0 iff [this] is the root
  std::size_t rank;
  boost::dynamic_bitset<> subtype;

public:
  // TODO Remove that
  // Used in unary case in mph.cpp
  const signature_t* const sig;

private:
  // for use by hierarchy_t
   klass_t(rtti_type id, std::size_t arity);
  ~klass_t();
  
public:
  rtti_type         get_id()       const { return id;       }
  bases_type const& get_bases()    const { return bases;    }

  boost::dynamic_bitset<>::reference is_pole() { return subtype[0]; }

public:
  // total hashing order
  struct hash_order
  {
    bool operator()(const klass_t& a, const klass_t& b) const
    { return a.id < b.id; }
  };
  // total[extended] subtyping order
  struct total_order
  {
    bool operator()(const klass_t& a, const klass_t& b) const
    { return a.rank > b.rank; }
  };
  // partial subtyping order
  struct subtypes
  {
    bool operator()(const klass_t& a, const klass_t& b) const
    { return a.subtype[b.rank]; }
  };
};

typedef std::vector<std::vector<const klass_t*> > pole_table_t;

#endif
