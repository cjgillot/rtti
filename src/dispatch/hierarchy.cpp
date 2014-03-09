#include <boost/tokenizer.hpp>

#include <boost/lexical_cast.hpp>
#include <vector>
#include <stack>

#include "hierarchy.hpp"

// readability improvement
#define is_pole subtype[0]

// ----- hierarchy ----- //

hierarchy_t::hierarchy_t()
: current_rank(0)
{}

typedef hierarchy_t::dict_t dict_t;

klass_t*
hierarchy_t::do_add(rtti_hierarchy vec) {
  if(!vec)
    return nullptr;

  std::size_t id = vec->id;

  dict_t::iterator dit = dict.find(id);
  if(dit != dict.end()) {
    klass_t* k = dit->second;

    assert( k->id == id );

    return k;
  }
  else {
    klasses.emplace_back( new klass_t( id, vec->arity ) );
    klass_t& k = *klasses.back();

    dict.insert(std::make_pair( id, &k ));
    for(std::size_t i = 0; i < vec->arity; ++i)
      k.bases[i] = do_add(vec->base[i]);

    return &k;
  }
}

void
hierarchy_t::shrink(std::vector<klass_t const*>& seq) {
  for(klass_t const* p0 : seq) {
    klass_t* p = const_cast<klass_t*>(p0);
    for(klass_t const*& b : p->bases)
      b = b->pole;

    std::sort(p->bases.begin(), p->bases.end());
    p->bases.erase( std::unique(p->bases.begin(), p->bases.end()), p->bases.end() );
    p->bases.erase( std::remove(p->bases.begin(), p->bases.end(), nullptr), p->bases.end() );
  }
}

klass_t const*
hierarchy_t::add(rtti_hierarchy vec) {
  klass_t* base = do_add(vec);
  const_cast<klass_t*>(base)->pole = base;
  const_cast<klass_t*>(base)->is_pole = true;
  return base;
}

void
hierarchy_t::pole_init(klass_t* k)
{
  std::size_t r = current_rank++;
  k->rank = r;

  k->subtype.set(r);

  for(klass_t const* b : k->bases)
    k->subtype |= b->subtype;
}

std::size_t
hierarchy_t::pseudo_closest(const klass_t* k, const klass_t*& pole)
{
  // compute candidates
  std::vector<klass_t const*> candidates;
  candidates.reserve(k->bases.size());

  for(klass_t const* base : k->bases)
    if(k->pole)
      candidates.push_back(k->pole);

  if(candidates.size() == 0)
    return 0;

  klass_t const* const maxK = *std::max_element(
    candidates.begin(), candidates.end(),
    [](klass_t const* a, klass_t const* b)
    { return a->rank < b->rank; }
  );

  for(klass_t const* k : candidates)
    if( !klass_t::subtypes()(*k, *maxK) )
      return 2;

  pole = maxK;
  return 1;
}

void hierarchy_t::order(std::vector<klass_t const*>& seq) {
  // primary poles are marked by add()

  /// hierarchy must be shrunk already
  std::vector<klass_t*> stack; stack.reserve(dict.size());
  std::transform(
    dict.begin(), dict.end(),
    std::back_inserter(stack),
    [](dict_t::reference p) { return p.second; }
  );

  // init primary poles
  for(klass_t* k : stack)
    if(k->is_pole) {
      pole_init(k);
      k->is_pole = false;
      
      seq.push_back(k);
    }

  // is_pole marks traversed state
  while(!stack.empty()) {
    klass_t* top = stack.back();

    // already visited
    if(top->is_pole) {
      stack.pop_back();
      continue;
    }

    // upcast if not visited yet
    for(;;) {
      bool need_upcast = false;
      for(klass_t const* base : top->bases)
        if(! base->is_pole) {
          need_upcast = true;
          stack.push_back( const_cast<klass_t*>(base) );
        }

      if(!need_upcast)
        break;
      
      stack.push_back(top);
      top = const_cast<klass_t*>(top->bases[0]);
    }

    // compute
    klass_t const* pole;
    std::size_t sz = pseudo_closest(top, pole);
    if(sz == 0)
      top->pole = nullptr;

    else if(sz == 1)
      top->pole = pole;

    else {
      pole_init(top);
      top->pole = top;

      seq.push_back(top);
    }

    // mark visited
    top->is_pole = true;
  }
  
  shrink(seq);
}
