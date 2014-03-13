#include <boost/tokenizer.hpp>

#include <boost/foreach.hpp>
#include <vector>
#include <stack>

#include "hierarchy.hpp"
#include "manip.hpp"

// readability improvement
#define is_pole subtype[0]

// ----- hierarchy ----- //

hierarchy_t::hierarchy_t()
: current_rank(0)
{}

hierarchy_t::~hierarchy_t()
{
  BOOST_FOREACH(klass_t* k, klasses)
    delete k;
}

typedef hierarchy_t::dict_t dict_t;

klass_t*
hierarchy_t::do_add(rtti_hierarchy vec) {
  if(!vec)
    return NULL;

  rtti_type id = rtti_get_id(vec);

  dict_t::iterator dit = dict.find(id);
  if(dit != dict.end()) {
    klass_t* k = dit->second;

    assert( k->id == id );

    return k;
  }
  else {
    std::size_t const arity = rtti_get_base_arity(vec);

    klasses.push_back( new klass_t( id, arity ) );
    klass_t& k = *klasses.back();

    dict.insert(std::make_pair( id, &k ));
    for(std::size_t i = 0; i < arity; ++i)
      k.bases[i] = do_add( rtti_get_base(vec, i) );

    return &k;
  }
}

namespace {

struct rank_compare {
  bool operator()(klass_t const* a, klass_t const* b) const
  { return a->rank < b->rank; }
};

struct select_second {
  template<typename Pair>
  typename Pair::second_type
    operator()(Pair const& b) const
  { return b.second; }
};

} // namespace

klass_t const*
hierarchy_t::add(rtti_hierarchy vec) {
  klass_t* base = do_add(vec);
  const_cast<klass_t*>(base)->pole = base;
  const_cast<klass_t*>(base)->is_pole = true;
  return base;
}

void
hierarchy_t::shrink(std::vector<klass_t const*>& seq) {
  BOOST_FOREACH(klass_t const* p0, seq) {
    klass_t* p = const_cast<klass_t*>(p0);
    BOOST_FOREACH(klass_t const*& b, p->bases)
      b = b->pole;

    std::sort(p->bases.begin(), p->bases.end());
    p->bases.erase( std::unique(p->bases.begin(), p->bases.end()), p->bases.end() );
    p->bases.erase( std::remove(p->bases.begin(), p->bases.end(), (klass_t const*)NULL), p->bases.end() );
  }
  
  std::sort(seq.begin(), seq.end(), rank_compare());
}

void
hierarchy_t::pole_init(klass_t* k)
{
  std::size_t r = current_rank++;
  k->rank = r;

  if(k->subtype.size() <= r)
    k->subtype.resize(1+r);

  BOOST_FOREACH(klass_t const* b, k->bases) {
    if(b->subtype.size() <= r)
      const_cast<klass_t*>(b)->subtype.resize(1+r);

    k->subtype |= b->subtype;
  }

  k->subtype.set(r);
}

std::size_t
hierarchy_t::pseudo_closest(const klass_t* k, const klass_t*& pole)
{
  // compute candidates
  std::vector<klass_t const*> candidates;
  candidates.reserve(k->bases.size());

  BOOST_FOREACH(klass_t const* base, k->bases)
    if(base->pole)
      candidates.push_back(base->pole);

  if(candidates.size() == 0)
    return 0;

  if(candidates.size() == 1) {
    pole = candidates.front();
    return 1;
  }

  klass_t const* const maxK = *std::max_element(
    candidates.begin(), candidates.end(),
    rank_compare()
  );

  BOOST_FOREACH(klass_t const* k, candidates)
    if( !klass_t::subtypes()(*k, *maxK) )
      return 2;

  pole = maxK;
  return 1;
}

namespace {

// is_pole is used as a traversal flag
struct wanderer_t {
  std::vector<klass_t const*> stack;

  wanderer_t(std::size_t dictsz)
  { stack.reserve(dictsz); }

  void push(klass_t const* k) {
    const_cast<klass_t*>(k)->is_pole = false;
    stack.push_back(k);
  }

  klass_t* pop() {
    for(;;) {
      if(stack.empty())
        return NULL;

      klass_t* top = const_cast<klass_t*>( stack.back() );
      stack.pop_back();

      if(top->is_pole)
        continue;

      top->is_pole = true;

      BOOST_FOREACH(klass_t const* base, top->bases)
        // not visited yet
        if(! base->is_pole)
          stack.push_back(base);

      // FIXME sure ?
      stack.erase( std::remove(stack.begin(), stack.end(), top), stack.end() );

      // already visited
      if(top->pole)
        continue;

      return top;
    }
  }
  bool empty() const { return stack.empty(); }
};

} // namespace <>

void hierarchy_t::order(std::vector<klass_t const*>& seq) {
  // primary poles are marked by add()

  /// hierarchy must be shrunk already
  wanderer_t wanderer (dict.size());
  std::transform(
    dict.begin(), dict.end(),
    std::back_inserter(wanderer.stack),
    select_second()
  );

  // FIXME just in case
  seq.clear();

  // init primary poles
  BOOST_FOREACH(klass_t const* k, wanderer.stack)
    if(k->is_pole) {
      BOOST_ASSERT( k->pole == k );
      klass_t* k2 = const_cast<klass_t*>(k);

      pole_init(k2);
      seq.push_back(k);
    }

  // traverse
  while(klass_t* top = wanderer.pop()) {
    BOOST_ASSERT( std::find(seq.begin(), seq.end(), top) == seq.end() );

    // compute
    klass_t const* pole;
    std::size_t sz = pseudo_closest(top, pole);
    if(sz == 0)
      top->pole = NULL;

    else if(sz == 1)
      top->pole = pole;

    else {
      pole_init(top);
      top->pole = top;

      seq.push_back(top);
    }
  }
  
  shrink(seq);
}
