#include "hierarchy.hpp"
#include "overloads.hpp"

#include <unordered_set>
#include <boost/range/adaptor/transformed.hpp>

#include <boost/optional.hpp>

namespace {

typedef std::vector<std::vector<const klass_t*>::const_iterator> product_t;

void product_alloc(product_t& p, const pole_table_t &table)
{
  p.reserve(table.size());
  std::transform(table.begin(), table.end(), std::back_inserter(p),
    [](const std::vector<const klass_t*> &v) { return v.begin(); }
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

auto product_deref(product_t& p) -> decltype(boost::adaptors::transform(p, deref()))
{
  return boost::adaptors::transform(p, deref());
}

}

#include <iostream>

#define PRINT_SIG(sig) do {                     \
  size_t __i = 0;                               \
  for(klass_t const* __k : (sig).array())       \
    std::cerr << (__i++>0?",":"") << __k->hash  \
              << '@' << __k->rank; \
} while(0)

static void dispatch_one(
  const signature_t& sig,
  const pole_table_t &pole_table,
  dispatch_t &dispatch
);

void dispatch(
  const overloads_t& overloads,
  const pole_table_t &pole_table,
  dispatch_t &dispatch
) {
  for(signature_t const& s : overloads.array())
    dispatch.insert({ s, s });

  product_t p;
  product_alloc(p, pole_table);

  // the order given by product_incr is
  // a topological order for signature_t::subtypes,
  // each base is dispatched before any of its derived
  do {
    signature_t sig { product_deref(p) };
    dispatch_one(sig, pole_table, dispatch);
  }
  while( product_incr(p, pole_table) );
}

struct end_loop {};

static signature_t const& sig_upcast(
  const signature_t &sig0,
  size_t& k, // next upcast index
  size_t arity,
  dispatch_t const& dispatch
) throw(end_loop);

static void dispatch_one(
  const signature_t& sig,
  const pole_table_t &pole_table,
  dispatch_t &dispatch
) {
  const size_t arity = pole_table.size();

  // already registered
  if(dispatch.find(sig) != dispatch.end())
    return;

  // current argument index
  size_t k = 0;

  // set of candidates
  std::unordered_set<signature_t> max_set;

  try {
    // insert first candidate
    max_set.insert( sig_upcast(sig, k, arity, dispatch) );

    for(;;) {
      // new candidate to be tested
      signature_t const& s2 = sig_upcast(sig, k, arity, dispatch);

      bool dominated = false;

      for(auto it = max_set.begin(), en = max_set.end(); it != en; ++it)
      {
      restart:
        signature_t const& e = *it;

        // [s2] is better match, remove [it]
        if( signature_t::subtypes()(e, s2) )
        { it = max_set.erase(it); goto restart; }

        // [it] is better match, don't insert [s2]
        else if( signature_t::subtypes()(s2, e) )
        { dominated = true; break; }
      }

      // none of [max_set] is better
      if( !dominated )
        max_set.insert(s2);
    }
  }
  catch(end_loop&) {}

  if(max_set.size() == 1)
    dispatch.insert({ sig, boost::make_optional(*max_set.begin()) });

  else {
    dispatch.insert({ sig, boost::none });
    if(max_set.size() == 0) {
      std::cerr << "No overload found for signature : ";
      PRINT_SIG(sig);
      std::cerr << std::endl;
    }
    else {
      std::cerr << "Ambiguity found for signature : ";
      PRINT_SIG(sig);
      std::cerr << std::endl << "note : candidates are : ";
      size_t ind = 0;
      for(signature_t const& candidate : max_set) {
        if(ind++>0)
          std::cerr << " ; ";
        PRINT_SIG(candidate);
      }
      std::cerr << std::endl;
    }
  }
}

static signature_t const& sig_upcast(
  signature_t const& sig0,
  size_t& k, size_t arity,
  dispatch_t const& dispatch
) throw(end_loop)
{
  for(;;) {
    signature_t sig = sig0;
    const klass_t** kl = nullptr;

    // find upcast the first derived class found
    do {
      // none remaining :
      if(k == arity) throw end_loop();

      kl = &sig.array_ref()[k++];
    }
    while((*kl)->base == nullptr);
    *kl = (*kl)->base;

    // we can safely use [dispatch.at] since all the candidates have been dispatched already
    boost::optional<signature_t> const& bound = dispatch.at(sig);
    if(bound) return *bound;
  }
}
