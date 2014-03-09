#include <vector>

#include <algorithm>
#include <functional>

#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include "signature.hpp"

// total[extended] subtyping order
bool signature_t::total_order::operator()(const signature_t& a, const signature_t& b) const
{
  return std::lexicographical_compare(
    a.sig.begin(), a.sig.end(),
    b.sig.begin(), b.sig.end(),
    [](const klass_t* aa, const klass_t* bb) { return klass_t::total_order()(*bb, *aa); }
  );
}

// partial subtyping order
bool signature_t::subtypes::operator()(const signature_t& a, const signature_t& b) const
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
    assert(it2 != en2); (void)en2;

    bool isder = f(**it1, **it2);
    bool isbase = f(**it2, **it1);
    if(!isder)
      return false;
    notallbase |= !isbase;
  }
  return notallbase;
}
