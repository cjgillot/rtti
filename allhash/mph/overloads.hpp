#ifndef RTTI_MPH_OVERLOADS_HPP
#define RTTI_MPH_OVERLOADS_HPP

#include <vector>

#include <algorithm>
#include <functional>

#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include "hierarchy.hpp"

class overloads_t;
class signature_t
{
  friend class overloads_t;
  std::vector<const klass_t*> sig;

public:
  explicit signature_t(std::size_t a) { sig.resize(a); }

  template<typename Range>
  explicit signature_t(const Range &v)
  : sig(std::begin(v), std::end(v)) {}

  signature_t(const signature_t&) = default;
  signature_t& operator=(const signature_t&) = default;

  signature_t(signature_t&&) = default;
  signature_t& operator=(signature_t&&) = default;

public:
  std::vector<const klass_t*>& array() { return sig; }
  const std::vector<const klass_t*>& array() const { return sig; }

public:
  // total[extended] subtyping order
  struct total_order
  {
    bool operator()(const signature_t& a, const signature_t& b) const
    {
      return std::lexicographical_compare(
        a.sig.begin(), a.sig.end(),
        b.sig.begin(), b.sig.end(),
        [](const klass_t* aa, const klass_t* bb) { return klass_t::total_order()(*bb, *aa); }
      );
    }
  };
  // partial subtyping order
  struct subtypes
  {
    bool operator()(const signature_t& a, const signature_t& b) const
    {
      assert(a.sig.size() == b.sig.size());
      // return true if [b] is better overload than [a]
      // ie. if [\forall i, a_i <: b_i \and \exists_i, \not(b_i <: a_i)]
      klass_t::subtypes f;
      bool notallbase = false;
      for(auto it1 = a.sig.begin(),
               it2 = b.sig.begin(),
               en1 = a.sig.end(),
               en2 = b.sig.end();
          it1 != en1;
          ++it1, ++it2)
      {
        assert(it2 != en2);

        bool isder = f(**it1, **it2);
        bool isbase = f(**it2, **it1);
        if(!isder)
          return false;
        notallbase |= !isbase;
      }
      return notallbase;
    }
  };

  friend bool operator==(const signature_t& a, const signature_t& b)
  { return a.sig == b.sig; }
};

class overloads_t
{
  std::size_t arity;
  std::vector<signature_t> values;

public:
  explicit
  overloads_t(std::size_t arity);

  void save();
  void finish();

  void sort();

public:
  const klass_t* &get_back(std::size_t i)
  { return values.back().sig[i]; }

  const std::vector<signature_t>& array() const { return values; }
};

typedef std::map<signature_t, boost::optional<signature_t>, signature_t::total_order> dispatch_t;

namespace std {
  template<>
  struct hash<signature_t>
  {
    std::size_t operator()(const signature_t& s) const
    {
      return boost::hash_range(s.array().begin(), s.array().end());
    };
  };
}

#endif
