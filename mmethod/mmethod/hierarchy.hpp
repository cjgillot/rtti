#ifndef RTTI_MPH_HIERARCHY_HPP
#define RTTI_MPH_HIERARCHY_HPP

#include <cstddef>
#include <unordered_map>

#include <boost/dynamic_bitset.hpp>

#include "archive.hpp"

class hierarchy_t;
class signature_t;
struct klass_t
{
  friend class hierarchy_t;

  std::size_t hash;
  klass_t* base;

  /// \invariant : subtype[o.rank] = 1 - iff [this] derives from [o]
  /// \invariant : subtype[rank] = 1 - consequence
  /// \invariant : subtype[0] is pole flag then : rank == 0 iff [this] is the root
  std::size_t rank;
  boost::dynamic_bitset<> subtype;

  std::size_t rankhash;

  const signature_t* const sig;

public:
  klass_t();

public:
  // remove non-pole in hierarchy
  void shrink();

public:
  // total hashing order
  struct hash_order
  {
    bool operator()(const klass_t& a, const klass_t& b) const
    { return a.hash < b.hash; }
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

class hierarchy_t
{
public:
  typedef std::unordered_map<std::size_t, klass_t> dict_t;
  dict_t dict;

public:
  // remove non-pole in hierarchy
  void shrink();

  const klass_t* add(hashvec_t const& vec);

  template<typename Sequence>
  void order(Sequence& seq);

public:
  std::size_t size() const { return dict.size(); }
};

typedef std::vector<std::vector<const klass_t*>> pole_table_t;

#endif
