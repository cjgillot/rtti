//          Copyright Camille Gillot 2012 - 2015.
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

  rtti_hierarchy const rtti;
  bases_type bases;

  /// \invariant : subtype[o.rank] = 1 - iff [this] derives from [o]
  /// \invariant : subtype[rank] = 1 - consequence
  std::size_t rank;
  boost::dynamic_bitset<> subtype;

private:
  // for use by hierarchy_t
  explicit klass_t(rtti_hierarchy hh);
  ~klass_t();

  // setup rank & subtype
  void set_rank(std::size_t rank);

public:
  rtti_type         get_id()     const { return rtti_get_id(rtti);      }
  rtti_hierarchy    get_rtti()   const { return rtti;                   }
  bases_type const& get_bases()  const { return bases;                  }
  std::size_t       get_rank()   const { return rank;                   }

public:
  // total hashing order
  struct hash_order
  {
    bool operator()(const klass_t& a, const klass_t& b) const
    { return a.rtti < b.rtti; }
  };
  // total[extended] subtyping order - small is subtype
  struct total_order
  {
    bool operator()(const klass_t& a, const klass_t& b) const
    { return a.rank > b.rank; }
  };
  // partial subtyping order - small is subtype
  struct is_subtype_of
  {
    bool operator()(const klass_t& a, const klass_t& b) const
    { return a.subtype[b.rank]; }
  };
};

#endif
