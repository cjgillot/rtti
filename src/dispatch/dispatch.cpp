//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"
#include "overloads.hpp"

#include <list>

#include <boost/range/adaptor/transformed.hpp>

#include <boost/range/begin.hpp>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

namespace {

typedef std::vector<const klass_t*> kvector_t;
typedef std::vector<kvector_t::const_iterator> product_t;

struct beginner {
  kvector_t::const_iterator operator()(kvector_t const& p) const
  { return p.begin(); }
};

void product_alloc(product_t& p, const pole_table_t &table)
{
  p.reserve(table.size());
  std::transform(
    table.begin(), table.end(),
    std::back_inserter(p),
    beginner()
  );
}

bool product_incr(product_t& p, const pole_table_t &table)
{
  for(std::size_t k = 0; k < p.size(); ++k)
  {
    ++p[k];
    if( p[k] != table[k].end() ) return true;
    p[k] = table[k].begin();
  }
  return false;
}

struct deref
: std::unary_function<std::vector<const klass_t*>::const_iterator, const klass_t*>
{
  result_type operator()(const argument_type& it) const
  { return *it; }
};

boost::transformed_range<deref, product_t>
product_deref(product_t& p)
{
  return boost::adaptors::transform(p, deref());
}

} // namespace <>

static void dispatch_one(
  const signature_t& sig,
  const pole_table_t &pole_table,
  dispatch_t &dispatch
);

void dispatch(
  dispatch_t &dispatch,
  overloads_t& overloads,
  const pole_table_t &pole_table
) {
// FIXME Why ?
//   std::sort(overloads.begin(), overloads.end());

  BOOST_FOREACH(overload_t const& s, overloads)
    dispatch.insert(std::make_pair( s.first, s ));

  product_t p;
  product_alloc(p, pole_table);

  // the order given by product_incr is
  // a topological order for signature_t::subtypes,
  // each base is dispatched before any of its derived
  do {
    signature_t sig ( product_deref(p) );
    dispatch_one(sig, pole_table, dispatch);
  }
  while( product_incr(p, pole_table) );
}

struct end_loop {};

namespace {

struct sig_upcaster {
  signature_t const& sig0;
  std::size_t const arity;
  dispatch_t  const& dispatch;
  
  std::size_t k;
  std::size_t b;
  
  sig_upcaster(signature_t const& s0, std::size_t a, dispatch_t const& d);
  overload_t const* operator()() BOOST_NOEXCEPT_OR_NOTHROW;
};

} // namespace <>

static void dispatch_one(
  const signature_t& sig,
  const pole_table_t &pole_table,
  dispatch_t &dispatch
) {
  const size_t arity = pole_table.size();

  // already registered
  if(dispatch.find(sig) != dispatch.end())
    return;

  sig_upcaster upcast ( sig, arity, dispatch );

  // set of candidates
  typedef std::list<overload_t> max_set_type;
  max_set_type max_set;

  for(;;) {
    // new candidate to be tested
    overload_t const* up = upcast();

    // end loop ?
    if(!up)
      break;

    bool dominated = false;

    for(max_set_type::iterator it = max_set.begin(), en = max_set.end(); it != en; ++it)
    {
    filter:
      overload_t const& e = *it;

      // [*up] is better match, remove [it]
      if( signature_t::subtypes()(e.first, up->first) )
      { it = max_set.erase(it); goto filter; }

      // [it] is better match, don't insert [s2]
      else if( signature_t::subtypes()(up->first, e.first) )
      { dominated = true; break; }
    }

    // none of [max_set] is better
    if( !dominated )
      max_set.push_back(*up);
  }

  if(max_set.size() == 1)
    dispatch.insert(std::make_pair( sig, boost::make_optional(*max_set.begin()) ));

  else {
    //FIXME : diagnose
    dispatch.insert(std::make_pair( sig, boost::none ));
//     if(max_set.size() == 0) {
//       std::cerr << "No overload found for signature : ";
//       PRINT_SIG(sig);
//       std::cerr << std::endl;
//     }
//     else {
//       std::cerr << "Ambiguity found for signature : ";
//       PRINT_SIG(sig);
//       std::cerr << std::endl << "note : candidates are : ";
//       size_t ind = 0;
//       for(overload_t const& candidate : max_set) {
//         if(ind++>0)
//           std::cerr << " ; ";
//         PRINT_SIG(candidate.first);
//       }
//       std::cerr << std::endl;
//     }
  }
}

sig_upcaster::sig_upcaster(const signature_t& s0, std::size_t a, const dispatch_t& d)
: sig0(s0), arity(a), dispatch(d)
, k(0), b(0) {}

overload_t const*
sig_upcaster::operator()() BOOST_NOEXCEPT_OR_NOTHROW
{
  for(;;) {
    signature_t sig = sig0;

    // continue upcast on base [b] of argument [k]
    klass_t const* nk = NULL;
    while(!nk) {
      // proceed to next argument
      while( b == sig.array()[k]->get_bases().size() ) {
        b = 0; ++k;
        if(k == arity)
          return NULL;
      }

      // select
      nk = sig.array()[k]->get_bases()[b];

      // prepare for next base
      ++b;
    }

    sig.array_ref()[k] = nk;

    // we can safely use [dispatch.at] since all the candidates have been dispatched already
    boost::optional<overload_t> const& bound = dispatch.at(sig);
    
    // FIXME may hide ambiguity cascade
    if(bound) return &*bound;
  }
}
