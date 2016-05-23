//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "signature.hpp"
#include "hierarchy.hpp"

// worse_match order
bool signature_t::worse_match::operator()(const signature_t& a, const signature_t& b) const
{
  BOOST_ASSERT(a.sig.size() == b.sig.size());

  // return true if [b] is better overload than [a]
  // ie. if :
  //   \forall i, b_i <: a_i (a is more general than b)
  // and
  //   \exists_i, \not(a_i <: b_i) (b is not more general than a)
  klass_t::is_subtype_of f;
  bool notallbase = false;

  typedef sig_type::const_iterator it_type;
  for(it_type it1 = a.sig.begin(),
              it2 = b.sig.begin(),
              en1 = a.sig.end(),
              en2 = b.sig.end();
      it1 != en1;
      ++it1, ++it2)
  {
    BOOST_ASSERT(it2 != en2); (void)en2;

    klass_t const* const klass1 = *it1;
    klass_t const* const klass2 = *it2;

    bool const isder  = f(*klass2, *klass1);
    bool const isbase = f(*klass1, *klass2);

    // \forall_i b_i <: a_i
    if(!isder) {
      return false;
    }

    // \exists_i \not(a_i <: b_i)
    notallbase |= !isbase;
  }

  return notallbase;
}

namespace {

struct hierarchy_adder {
  klass_t const* operator()(rtti_hierarchy a0, hierarchy_t& a1) const
  {
    klass_t const* ret = a1.fetch(a0);
    BOOST_ASSERT(ret);
    return ret;
  }
};

} // namespace

signature_t make_signature(rtti_signature const& r0,
                           pole_table_t& r1)
{
  signature_t ret ( r1.size() );

  std::transform(
    r0.begin(), r0.end(),
    r1.begin(),

    ret.array_ref().begin(),

    hierarchy_adder()
  );

  return ret;
}
